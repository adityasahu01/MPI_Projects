#!/bin/python3

############################################################
#   Author - Aditya Sahu
#   https://github.com/adityasahu01/MPI_Projects
#   
#   Copyright (c), 2023
#############################################################

import os
import subprocess
import argparse
import sys
import multiprocessing
import logging
from datetime import datetime
from prettytable import PrettyTable

exec_dict = {
    "test_multiply_vector" : "multiply_vector",
    "test_add_vector" : "add_vector"
}

src_path = [
    "vector_mult",
    "vector_add"
]

script_dir = os.path.dirname(os.path.abspath(__file__))
tc_file = "testcases"
log_file = ""

# Stats for testcases
run = 0
failed = 0
passed = 0
non_zero = 0

failed_testcase = list()

def print_summary():
    table = PrettyTable()
    table.field_names = ["TC Info", "#"]
    table.add_row(["Total TCs", run])
    table.add_row(["Passed", passed])
    table.add_row(["Failed", failed])
    table.add_row(["Non-Zero returns", non_zero])
    
    table_str = str("Testsuite summary:\n" + str(table))
    
    print(table_str)
    logging.info(table_str)
    
    print("Failed Test Cases : ", failed_testcase)
    logging.info(str("Failed Test Cases : " + str(failed_testcase)))
    return
    

def run_command(command):
    try:
        result = subprocess.run(command, 
                                stdout=subprocess.PIPE, 
                                stderr=subprocess.PIPE, 
                                check=True, text=True)
        
    except subprocess.CalledProcessError as e:
        return_code = e.returncode
        stderr = e.stderr

        print(f"{command} command failed with exit code: {return_code}")
        print(f"Standard Error:\n{stderr}")
        logging.error(f"{command} stderr:\n%s", stderr.encode().decode())
        logging.error(f"{command} exited with non-zero status code: {e.returncode}")
        logging.error("Exiting!")
        sys.exit(-1) 
    
    return result

def initialize_logging():
    global log_file
    timestamp = datetime.now().strftime("%d%m%y_%H%M%S")
    # Construct the log file name
    log_file = f"logs_{timestamp}.txt"
    
    # Configure logging
    logging.basicConfig(filename=log_file, 
                        level=logging.INFO, 
                        format='%(asctime)s - %(levelname)s - %(message)s')

def read_testcases(tc_file, testcase):
    testcases = dict()
    lines = list()
    
    testcase = testcase if testcase else 'test'
    
    with open(tc_file) as f:
        lines = f.read().splitlines()
    
    for line in lines:
        if line.startswith(testcase):
            line = line.split(' ')
        else:
            continue
        
        # key maps to test executable
        # value is the argument to be passed
        key = exec_dict[line[0]]
        value = line[1]
        
        if key in testcases:
            testcases[key].append(value)
        else:
            testcases[key] = [value]
    
    print(testcases)
    return testcases

def setup_ws():
    build_dirs = list()
    
    for src_dir in src_path:
        src_dir = os.path.abspath(os.path.join(script_dir, os.pardir, src_dir))
        print("Source dir :", src_dir)

        build_dirs.append(os.path.join(src_dir, 'build'))
        print(build_dirs)
        
        if os.path.exists(src_dir):
            os.chdir(src_dir)
            logging.info(f"Switched to directory '{src_dir}'")
        else:
            print(f"Target directory '{src_dir}' does not exist.")
            logging.error(f"Target directory '{src_dir}' does not exist.")
            sys.exit(-1)
        
        # Run cmake to configure the build
        cmake_command = ["cmake", src_dir]
        rc = run_command(cmake_command)
        
        return_code = rc.returncode
        stdout = rc.stdout

        print(f"CMake returned with exit code: {return_code}")
        print(f"Standard Output:\n{stdout}")
        logging.info(f"CMake completed successfully! {src_dir}")
        logging.info(f"CMake stdout:\n%s", stdout.encode().decode())
        

        # Run make to build the project
        make_command = ["make"]
        rc = run_command(make_command)
        
        return_code = rc.returncode
        stdout = rc.stdout

        print(f"Make returned with exit code: {return_code}")
        print(f"Standard Output:\n{stdout}")
        logging.info(f"Make completed successfully! {src_dir}")
        logging.info(f"Make stdout:\n%s", stdout.encode().decode())

    return build_dirs

def run_single_test(executable, arg):
    logging.info(f"Running Test '{executable} {arg}'")
    print((f"Running Test '{executable} {arg}'"))

    rc = 0
    
    '''
    TODO -
    -   Look for unwanted outputs "Outputs don't match"
    -   Look for non-zero return codes
    '''
    test_command = [executable, str(arg)]
    result = run_command(test_command)

    # Capture and log standard output
    stdout = result.stdout.strip()
    if stdout:
        logging.info(f"Test '{executable} {arg}' \n{stdout}")

    # Capture and log standard error
    stderr = result.stderr.strip()
    if stderr:
        logging.error(f"Test '{executable} {arg}' \n{stderr}")

    # Put assert check here
    # Step 1
    if result.returncode != 0:
        rc = 1
        # print(f"Test '{executable} {arg}' returned Non-Zero rc : {result.returncode}")
        logging.info(f"Test '{executable} {arg}' returned Non-Zero rc : {result.returncode}")
        print(f"Test '{executable} {arg}' returned Non-Zero rc : {result.returncode}")
        return rc
    else:
        print(f"Test '{executable} {arg}' passed step 1")
        logging.info(f"Test '{executable} {arg}' passed step 1")
    
    failed_output = "don't match"
    if stdout.find(failed_output) == -1:
        rc = 0
        print(f"Test '{executable} {arg}' passed")
        logging.info(f"Test '{executable} {arg}' passed")
    else:
        rc = -1
        failed_testcase.append(test_command)
        logging.info(f"Test '{executable} {arg}' failed")
        assert False, f"Test '{executable} {arg}' failed"
    
    return rc

def run_tests(testcases, build_dirs):
    max_processes = multiprocessing.cpu_count()
    pool = multiprocessing.Pool(processes=max_processes)
    running_processes = list()
    global run
    global passed
    global failed
    global non_zero

    for (executable, args), path in zip(testcases.items(), build_dirs):
        logging.info(f"Running Test '{executable}'")
        
        exec_path = os.path.join(path, executable)
        
        for arg in args:
            run += 1
            # Check if we have reached the maximum number of concurrent processes
            while len(running_processes) >= max_processes:
                # Wait for a process to finish before adding a new one
                finished_process = multiprocessing.Process(target=lambda: None)
                finished_process.start()
                finished_process.join()
                running_processes.pop(0)

            # Start a new process for the test
            process = pool.apply_async(run_single_test, (exec_path, arg))
            running_processes.append(process)
        
            rc = process.get()
            if rc == 0:
                passed += 1
            elif rc == -1:
                failed += 1
            else:
                non_zero += 1
        
        logging.info(f"Finished Test '{executable}'")

    # Wait for all processes to complete
    for process in running_processes:
        process.wait()

    # Close the pool
    pool.close()
    pool.join()
    
    print_summary()

    logging.info("All tests have been run")
    return

def clean(build_dirs):
    for src_dir in build_dirs:
        ws = os.path.dirname(os.path.abspath(src_dir))
        
        if os.path.exists(ws):
            os.chdir(ws)
            logging.info(f"Switched to directory '{ws}'")
        else:
            print(f"Target directory '{ws}' does not exist.")
            logging.error(f"Target directory '{ws}' does not exist.")
        
        clean_command = ["make", "clean_all"]
        result = run_command(clean_command)
        
        return_code = result.returncode
        stdout = result.stdout
        
        print(f"make clean_all command returned with exit code: {return_code}")
        print(f"Standard Output:\n{stdout}")
        logging.info("Workspace cleaned successfully")
        logging.info("Make stdout:\n%s", stdout.encode().decode())
    
    return
        

def main(args):
    initialize_logging()
    testcases_path = os.path.join(script_dir, "testcases")
    
    if args.testcase is not None:
        if args.testcase not in exec_dict:
            logging.error(f"No TCs found for `{args.testcase}`")
            sys.exit(-1)
    
    logging.info(f"Registering testcases from `{testcases_path}`")
    testcases = read_testcases(testcases_path, args.testcase)
    logging.info(f"Testcases registered successfully!")
    
    logging.info(f"Setting up the workspace")
    
    build_dirs = setup_ws()
    print(build_dirs)
    
    # if arg.all then run all testcases
    # if specific group mentioned then run TC
    logging.info(f"Running testcases")
    run_tests(testcases, build_dirs)
    
    if args.clean:
        clean(build_dirs)
    
    return

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    
    '''
    Usage - 
        -   ./test.py --testcase testcase_name --clean
        -   ./test.py --testcase test_multiply_vector --clean
        -   ./test.py --clean
    '''
    
    parser.add_argument('-t','--testcase', help="Testcase - defined in ./testcases")
    parser.add_argument('-A', '--all', help="Run all the testsuits", action='store_true')
    parser.add_argument('-c', '--clean', help="Clean the workspace", action='store_true')
    
    args = parser.parse_args()
    
    if args.testcase is not None and args.all:
        parser.error("You cannot specify --testcase and --all arguments simultaneously.")
    
    main(args)
    print(f"Logs written to {log_file}")