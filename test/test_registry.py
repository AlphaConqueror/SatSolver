
import os

# forall Sat-instances run an Sat/Unsat-test, a format-checking test and an assignement-checking test
# forall Unsat-instances run Sat/Unsat-test.


abs_inst_path = "inst"

rel_bin_path = "bin/satsolver"
rel_test_path = "bin/testrunner"
rel_out_path = "build/tmp.out"
rel_tmp_path = "build/tmp.in"

fail_msg_rc = "Unexpected return code %d, expected was %d"
fail_msg_satbin = "'satsolver' binary not available"
fail_msg_testbin = "'testrunner' binary not available"
fail_msg_output = "Output file not available"
fail_msg_inst = "Instance not available"
fail_msg_ppl = "Unexpected result"
fail_msg_format = "Invalid output file format"
fail_msg_assignm = "Assignment is not satisfying"
fail_msg_ple = "Pure Literal Elimination unsuccessful"
fail_msg_collect = "Unexpected return code for at least one tested instance"
fail_msg_no_test = "Tests missing"

rc_sat = 10
rc_unsat = 20

all_tests = {}
all_tests_path = {}

def join_check_path(tu, rel_path, msg):
    abs_path = tu.join_base(rel_path)
    if os.path.exists(abs_path):
        return abs_path
    else:
        raise FileNotFoundError(msg)

def get_test_conf(test_name):
    return test_name.split(".",3)[0]

def get_test_task(test_name):
    return test_name.split(".",3)[1]

def get_test_kind(test_name):
    return test_name.split(".",3)[2]

def get_test_id(test_name):
    return test_name.split(".",3)[3]

def get_test_file(test_name):
    return get_test_id(test_name)

def get_test_path(conf, tag, sat, filename):    
    return os.path.join(abs_inst_path, conf, tag, "sat" if sat else "unsat", filename)

def get_check_sat(tu):
    return join_check_path(tu, rel_bin_path, fail_msg_satbin)

def get_check_test(tu):
    return join_check_path(tu, rel_test_path, fail_msg_testbin)

def get_check_out(tu):
    return join_check_path(tu, rel_out_path, fail_msg_output)

def get_check_inst(tu, test_name):
    return join_check_path(tu, all_tests_path[get_test_id(test_name)], fail_msg_inst)

def get_rem_tmp(tu):
    tmp_path = tu.join_base(rel_tmp_path)
    if os.path.exists(tmp_path):
        os.remove(tmp_path)
    return tmp_path
    
def get_rem_out(tu):
    out_path = tu.join_base(rel_out_path)
    if os.path.exists(out_path):
        os.remove(out_path)
    return out_path
    
def ppl_test(tu, test_name):
    try:
        splitted = test_name.split(".",3)
        in_path = join_check_path(tu,
                                  get_test_path(get_test_conf(test_name), "ppl", True, get_test_file(test_name)),
                                  fail_msg_inst)
        sat_bin = get_check_sat(tu)
        out_path = get_rem_out(tu) 

        # run the solver with the only-print-pl option 
        tu.run(sat_bin, [in_path, out_path, "-p"])
    
        test_bin = get_check_test(tu)

        run_res = tu.run(test_bin, [test_name])
        rc, _, _ = run_res
        return tu.check(run_res, fail_msg_ppl)
    except FileNotFoundError as ex:
        return tu.FAILURE(str(ex))

def rc_test(tu, in_path, sat):
    try:
        sat_bin = get_check_sat(tu)
        out_path = get_rem_out(tu)
        
        run_res = tu.run(sat_bin, [in_path, out_path])
        rc, _, _ = run_res
        exp_rc = rc_sat if sat else rc_unsat
        return tu.check(run_res,
                        fail_msg_rc%(rc,exp_rc),
                        exp_rc)
    except FileNotFoundError as ex:
        return tu.FAILURE(str(ex))
    
def rc_unsat_test(tu, test_name):
    in_path = get_check_inst(tu, test_name)
    return rc_test(tu, in_path, False)

def rc_sat_test(tu, test_name):
    in_path = get_check_inst(tu, test_name)
    return rc_test(tu, in_path, True)


def format_test(tu, test_name, sat):
    try:
        in_path = get_check_inst(tu, test_name)
        check_rc = rc_test(tu, in_path, sat)
        test_bin = get_check_test(tu)
    
        run_res = tu.run(test_bin, [("sat:" + in_path) if sat else "unsat"])
        return tu.check(run_res, fail_msg_format)
    except FileNotFoundError as ex:
        return tu.FAILURE(str(ex))

def format_sat_test(tu, test_name):
    return format_test(tu, test_name, True)

def format_unsat_test(tu, test_name):
    return format_test(tu, test_name, False)

def parse_cnf(in_path):
    res = []
    with open(in_path, "r") as f:
        # ignore comments and the following specification line
        condition = True
        while condition:
            line = f.readline()
            condition = line[0] == 'c'
        for line in f:
            clause = line.split()
            clause.pop() # remove trailing zero
            clause = [int(x) for x in clause]
            if clause:
                res.append(clause)
    return res

def parse_assignm(out_path):
    with open(out_path, "r") as f:
        f.readline() # ignore first line
        res = f.readline().split()
        res = [int(x) for x in res]
        res.pop() # remove trailing zero
    return res

def cnf_count_vars(cnf):
    var = 0
    for clause in cnf:
        for x in clause:
            var = abs(x) if abs(x) > var else var
    return var

def cnf_app_assignm(cnf, assignm):
    for x in assignm:
        cnf.append([x,x,x])
    return cnf

def print_cnf(out_path, cnf):
    with open(out_path, "w") as f:
        f.write("p cnf " + str(cnf_count_vars(cnf)) + " " + str(len(cnf)) + "\n")
        for clause in cnf:
            for x in clause:
                f.write(str(x) + " ")
            f.write("0\n") # don't forget the trailing zero
            
def assignm_test(tu, test_name):
    try:
        # TODO: remove double execution of test
        check_format = format_test(tu, test_name, True)
        if not check_format:
            return check_format

        in_path = get_check_inst(tu, test_name)
        out_path = get_check_out(tu)
        tmp_path = get_rem_tmp(tu)
    
        cnf = parse_cnf(in_path)
        assignm = parse_assignm(out_path)
        assignm_cnf = cnf_app_assignm(cnf,assignm)
        print_cnf(tmp_path, assignm_cnf)
        os.remove(out_path)
    
        sat_bin = get_check_sat(tu)    
        run_res = tu.run(sat_bin, [tmp_path, out_path])
        rc, _, _ = run_res
        return tu.check(run_res, fail_msg_assignm, rc_sat)
    except FileNotFoundError as ex:
        return tu.FAILURE(str(ex))


def find_sat_tests_of_file(filename, conf):
    tests = {}
    if conf == "public":
        tests[conf + ".satsolver.rc_sat." + filename] = rc_sat_test
        tests[conf + ".satsolver.format_sat." + filename] = format_sat_test
    tests[conf + ".satsolver.assignm_sat." + filename] = assignm_test
    return tests

def find_unsat_tests_of_file(filename, conf):
    tests = {}
    if conf == "public":
        tests[conf + ".satsolver.rc_unsat." + filename] = rc_unsat_test
        tests[conf + ".satsolver.format_unsat." + filename] = format_unsat_test
    return tests

def find_tests_conf(conf):
    tests = {}
    sat_path = os.path.join(abs_inst_path, conf, "auto", "sat")
    unsat_path = os.path.join(abs_inst_path, conf, "auto", "unsat")
    if os.path.exists(sat_path):
        with os.scandir(sat_path) as it:
            for entry in it:
                filename = entry.name
                tests.update(find_sat_tests_of_file(filename, conf))
                all_tests_path[filename] = get_test_path(conf, "auto", True, filename)
    if os.path.exists(unsat_path):
        with os.scandir(unsat_path) as it:
            for entry in it:
                filename = entry.name
                tests.update(find_unsat_tests_of_file(filename, conf))
                all_tests_path[filename] = get_test_path(conf, "auto", False, filename)
    # sort tests by tested functionality
    return {k: v for k,v in sorted(tests.items(), key=lambda item: item[0])}


def add_other_test(testname, test_func, tag, sat):
    all_tests[testname] = test_func
    all_tests_path[get_test_id(testname)] = get_test_path(get_test_conf(testname), tag, sat, get_test_file(testname))
    
add_other_test("public.print_pl.test.tiny_ppl", ppl_test, "ppl", True)
all_tests.update(find_tests_conf("public"))







timeout_secs = 10
