
# Minishell Tester

## Overview

The **Minishell Tester** is a comprehensive testing suite designed to validate the functionality of your Minishell implementation. It includes a variety of test cases, ranging from basic command execution to advanced features like pipes, redirections, and more. This tool helps ensure that your Minishell behaves as expected and adheres to the specifications.

## Structure

The directory structure of the tester is as follows:

```
hard-tests/
programs/
sample-txt/
test.sh
tests/
```

- **hard-tests/**: Contains more complex and edge-case scenarios.
- **programs/**: Contains scripts and executable programs used for testing.
- **sample-txt/**: Sample text files used for various test cases.
- **test.sh**: The main script to run the tests.
- **tests/**: Contains categorized test directories such as `appends`, `cat`, `cd`, etc.

## Running the Tests

To use the tester, follow these steps:

1. ## Installation
Clone the repository and navigate to the project directory:

```sh
git clone https://github.com/aabderrafie/minishell_tester.git
cd tester
```

2. **Run the Test Script**:
   To run all tests, execute:
   ```sh
   ./test.sh
   ```
   To run tests for a specific category, such as `cat` or `heredoc`, provide the category name as an argument:
   ```sh
   ./test.sh cat
   ./test.sh heredoc
   ```

3. **Provide the Path to Your Minishell**:
   When prompted, enter the relative or absolute path to your Minishell directory or executable:
   ```
   Enter a relative or absolute path to your minishell directory or executable:
   ```

4. **Enter Your Minishell Prefix**:
   When prompted, enter the prefix for your Minishell commands. For example:
   ```
   Enter your minishell prefix. In Bash it's 'bash', so you probably use 'minishell'
   ```

5. **Compilation**:
   The script will attempt to compile your Minishell. If successful, the tests will run automatically.

## Test Categories

The `tests/` directory contains various subdirectories, each focusing on a specific aspect of shell functionality:

- **appends/**: Tests related to output redirection in append mode (`>>`).
- **cat/**: Tests using the `cat` command.
- **cd/**: Tests for the `cd` command.
- **echo/**: Tests for the `echo` command with various options.
- **env/**: Tests for environment variable handling.
- **execution/**: General command execution tests.
- **exit/**: Tests for the `exit` command.
- **exit-status/**: Tests related to the exit status (`$?`).
- **export/**: Tests for the `export` command.
- **files/**: Tests involving file manipulations.
- **heredocs/**: Tests for here-document functionality (`<<`).
- **misc/**: Miscellaneous tests.
- **mkdir/**: Tests for the `mkdir` command.
- **nested-minishell/**: Tests for nested shell execution.
- **pipes/**: Tests for pipe functionality (`|`).
- **pwd/**: Tests for the `pwd` command.
- **redirections/**: Tests for input and output redirections.
- **rmdir/**: Tests for the `rmdir` command.
- **syntax-errors/**: Tests for various syntax errors.
- **unset/**: Tests for the `unset` command.
## Example
Below is an example of the tester output:
![Example Output](https://raw.githubusercontent.com/aabderrafie/minishell_tester/main/tester/example.png)

## Notes

- Ensure that your Minishell does not produce memory leaks. Use tools like `valgrind` to check for memory issues.
- The tests cover a wide range of scenarios, but it's still advisable to conduct manual testing to catch any edge cases not covered by the tester.

## Contributing

Contributions are welcome! If you have new test cases or improvements, feel free to fork the repository and submit a pull request.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

*Note: The provided script was not created by me.*
