#!/usr/bin/env python3

import random
import string
import subprocess
import os
import argparse
from pathlib import Path

DEFAULT_OUTPUT_DIR = Path("fuzzing_results")
DEFAULT_NUM_TESTS = 100
DEFAULT_MAX_LENGTH = 1000
OPAL_EXECUTABLE = Path("bin/opal")

SPECIAL_CHARS = "!@#$%^&*(){}[]<>:;,./?\\|~`'\""
OPERATORS = "+-*/%^=<>!&|#"
DIGITS = string.digits
LETTERS = string.ascii_letters
WHITESPACE = " \t\n\r"

ASCII_RANGE = (0x0020, 0x007E)  

class OpalFuzzer:
    def __init__(self, output_dir=DEFAULT_OUTPUT_DIR, verbose=False):
        self.output_dir = Path(output_dir)
        self.output_dir.mkdir(exist_ok=True, parents=True)
        self.verbose = verbose
        self.opal_executable = Path(os.getcwd()) / OPAL_EXECUTABLE
        
        if not self.opal_executable.exists():
            raise FileNotFoundError(f"Opal executable not found at: {self.opal_executable}")
            
        self.log_file = self.output_dir / "fuzzing_log.txt"
        with open(self.log_file, 'w') as f:
            f.write("=== Opal Fuzzing Log ===\n\n")
    
    def log(self, message):
        if self.verbose:
            print(message)
        with open(self.log_file, 'a') as f:
            f.write(f"{message}\n")
    
    def generate_random_char(self):
        return chr(random.randint(ASCII_RANGE[0], ASCII_RANGE[1]))
    
    def generate_string_literal(self):
        has_closing_quote = random.random() > 0.2  
        content_length = random.randint(0, 50)
        content = ''.join(random.choice(LETTERS + DIGITS + SPECIAL_CHARS + WHITESPACE) for _ in range(content_length))
        
        if random.random() > 0.7:
            escape_pos = random.randint(0, max(0, content_length - 1))
            escape_char = random.choice(r'\"nrt0\\')
            content = content[:escape_pos] + '\\' + escape_char + content[escape_pos:]
        
        if has_closing_quote:
            return f'"{content}"'
        else:
            return f'"{content}'
    
    def generate_number_literal(self):
        num_type = random.randint(0, 2)
        if num_type == 0:  
            return str(random.randint(-1000000, 1000000))
        elif num_type == 1:  
            return str(random.uniform(-1000000, 1000000))
        else:  
            components = []
            
            if random.random() > 0.7:
                components.append(random.choice('+-'))
            
            for _ in range(random.randint(1, 10)):
                if random.random() > 0.8:
                    components.append('.')
                else:
                    components.append(random.choice(DIGITS))
            
            if random.random() > 0.7:
                components.append('e')
                if random.random() > 0.5:
                    components.append(random.choice('+-'))
                for _ in range(random.randint(0, 5)):
                    components.append(random.choice(DIGITS))
            
            return ''.join(components)
    
    def generate_operator_sequence(self):
        length = random.randint(1, 5)
        return ''.join(random.choice(OPERATORS) for _ in range(length))
    
    def generate_identifier(self):
        length = random.randint(1, 30)
        first_char = random.choice(LETTERS + '_')
        rest = ''.join(random.choice(LETTERS + DIGITS + '_') for _ in range(length - 1))
        
        return first_char + rest
    
    def generate_comment(self):
        if random.random() > 0.5:
            content_length = random.randint(0, 50)
            content = ''.join(random.choice(LETTERS + DIGITS + SPECIAL_CHARS) for _ in range(content_length))
            return f'// {content}'
        else:
            content_length = random.randint(0, 100)
            content = ''.join(random.choice(LETTERS + DIGITS + SPECIAL_CHARS + '\n') for _ in range(content_length))
            has_closing = random.random() > 0.2  
            if has_closing:
                return f'/* {content} */'
            else:
                return f'/* {content}'
    
    def generate_fuzz_sample(self, max_length=DEFAULT_MAX_LENGTH):
        components = []
        current_length = 0
        
        while current_length < max_length:
            choice = random.randint(0, 5)  
            
            if choice == 0:
                component = self.generate_string_literal()
            elif choice == 1:
                component = self.generate_number_literal()
            elif choice == 2:
                component = self.generate_operator_sequence()
            elif choice == 3:
                component = self.generate_identifier()
            elif choice == 4:
                component = self.generate_comment()
            else:  
                component = ''.join(random.choice(WHITESPACE) for _ in range(random.randint(1, 5)))
            
            components.append(component)
            current_length += len(component)
            
            if random.random() > 0.7:
                delimiters = random.choice('(){}[];,.')
                components.append(delimiters)
                current_length += len(delimiters)
        
        return ''.join(components)[:max_length]
    
    def run_test(self, test_index, content):
        test_file = self.output_dir / f"test_{test_index}.op"
        try:
            with open(test_file, 'w', encoding='ascii', errors='replace') as f:
                f.write(content)
            
            result = subprocess.run(
                [str(self.opal_executable), str(test_file)], 
                capture_output=True, 
                text=True, 
                timeout=5  
            )
            
            status = "OK" if result.returncode == 0 else f"FAILED (code {result.returncode})"
            if result.returncode != 0:
                error_file = self.output_dir / f"error_{test_index}.txt"
                with open(error_file, 'w', encoding='ascii', errors='replace') as f:
                    f.write(f"STDOUT:\n{result.stdout}\n\nSTDERR:\n{result.stderr}")
            
            return status, result.returncode, len(content)
        except UnicodeEncodeError as e:
            self.log(f"Unicode encoding error in test {test_index}: {str(e)}")
            simple_content = ''.join(c if c in string.printable else '?' for c in content)
            with open(test_file, 'w', encoding='ascii', errors='replace') as f:
                f.write(simple_content)
            return self.run_test(test_index, simple_content)
        except subprocess.TimeoutExpired:
            self.log(f"Test {test_index}: TIMEOUT")
            return "TIMEOUT", -1, len(content)
        except Exception as e:
            self.log(f"Test {test_index}: EXCEPTION - {str(e)}")
            return f"EXCEPTION: {str(e)}", -2, len(content)
    
    def run_fuzzing_campaign(self, num_tests=DEFAULT_NUM_TESTS, max_length=DEFAULT_MAX_LENGTH):
        self.log(f"Starting fuzzing campaign: {num_tests} tests")
        
        results = {
            "total": num_tests,
            "success": 0,
            "failed": 0,
            "timeout": 0,
            "exception": 0
        }
        
        for i in range(1, num_tests + 1):
            content = self.generate_fuzz_sample(max_length)
            
            content = ''.join(c if ord(c) < 128 else '?' for c in content)
                
            status, retcode, length = self.run_test(i, content)
            
            if "OK" in status:
                results["success"] += 1
                status_type = "SUCCESS"
            elif "TIMEOUT" in status:
                results["timeout"] += 1
                status_type = "TIMEOUT"
            elif "EXCEPTION" in status:
                results["exception"] += 1
                status_type = "EXCEPTION"
            else:
                results["failed"] += 1
                status_type = "FAILED"
            
            self.log(f"Test {i}/{num_tests}: {status_type} - {status} (Size: {length})")
            
            if i % 10 == 0 and self.verbose:
                print(f"Progress: {i}/{num_tests} tests completed")
        
        self.generate_report(results)
        return results
    
    def generate_report(self, results):
        report_file = self.output_dir / "fuzzing_report.md"
        
        with open(report_file, 'w') as f:
            f.write("# Fuzzing Report for Opal Lexer\n\n")
            f.write(f"Execution date: {os.popen('date').read().strip()}\n\n")
            
            f.write("## Results\n\n")
            f.write(f"- **Total tests**: {results['total']}\n")
            f.write(f"- **Successful tests**: {results['success']} ({results['success']/results['total']*100:.1f}%)\n")
            f.write(f"- **Failed tests**: {results['failed']} ({results['failed']/results['total']*100:.1f}%)\n")
            f.write(f"- **Timeouts**: {results['timeout']} ({results['timeout']/results['total']*100:.1f}%)\n")
            f.write(f"- **Exceptions**: {results['exception']} ({results['exception']/results['total']*100:.1f}%)\n\n")
            
            f.write("## Observations\n\n")
            
            if results['failed'] > 0:
                f.write("Failed tests indicate situations where the lexer returned an error.\n")
                f.write("This may be normal (correctly detecting invalid input) or may reveal potential bugs.\n\n")
            
            if results['timeout'] > 0:
                f.write("Timeouts may indicate performance issues or infinite loops.\n\n")
            
            if results['exception'] > 0:
                f.write("Exceptions are typically unhandled errors and should be examined.\n\n")
            
            f.write("## Next Steps\n\n")
            f.write("1. Examine failed tests to identify common patterns\n")
            f.write("2. Check performance issues for timeout cases\n")
            f.write("3. Fix identified bugs and improve lexer robustness\n")
            f.write("4. Re-run tests to validate fixes\n")
        
        self.log(f"Report generated: {report_file}")

def main():
    parser = argparse.ArgumentParser(description="Opal Fuzzer - Fuzzing tool for the Opal lexer")
    parser.add_argument("--output-dir", "-o", type=str, default=DEFAULT_OUTPUT_DIR,
                        help=f"Output directory for results (default: {DEFAULT_OUTPUT_DIR})")
    parser.add_argument("--num-tests", "-n", type=int, default=DEFAULT_NUM_TESTS,
                        help=f"Number of tests to run (default: {DEFAULT_NUM_TESTS})")
    parser.add_argument("--max-length", "-m", type=int, default=DEFAULT_MAX_LENGTH,
                        help=f"Maximum size of test samples (default: {DEFAULT_MAX_LENGTH})")
    parser.add_argument("--verbose", "-v", action="store_true",
                        help="Verbose mode - display more information during execution")
    parser.add_argument("--use-unicode", action="store_true",
                        help="Include unicode characters in tests (may cause encoding issues)")
    
    args = parser.parse_args()
    
    try:
        fuzzer = OpalFuzzer(output_dir=args.output_dir, verbose=args.verbose)
        results = fuzzer.run_fuzzing_campaign(num_tests=args.num_tests, max_length=args.max_length)
        
        print("\nFuzzing campaign completed!")
        print(f"Summary: {results['success']} successful, {results['failed']} failed, {results['timeout']} timeouts, {results['exception']} exceptions")
        print(f"Report generated in: {Path(args.output_dir) / 'fuzzing_report.md'}")
        
    except Exception as e:
        print(f"Error: {str(e)}")
        return 1
    
    return 0

if __name__ == "__main__":
    exit(main())