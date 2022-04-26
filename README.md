# uat - universal avida tests

This repository contains implementation- and language-agnostic test cases for [Avida](https://github.com/devosoft/avida). This allows different implmentations to ensure they behave identically when executing an Avida organism with a given genome and hardware values. 

Currently, tests exist for the [default Avida instruction set](https://github.com/devosoft/avida/wiki/Instruction-Set), except for `h-alloc`, `h-divide`, `h-copy`, `h-search`, and `if-label`. 

Additionally, the repo contains interpreters for uat files for certain programming languages (currently Python and C++). These interpreters handle the files, so users only need to define a few functions specific to their Avida implementation to begin running uat files. 

## Test file structure
Test files are plaintext, and follow a very simple formula: 

```KEYWORD arg1 arg2```

The keywords are defined below. Arguments are either unsigned integers, chars (surrounded by 'single quotes'), strings (surrounded by "double quotes"), or values (whatever values are used by your Avida implementation, often unsigned integers).


| Keyword  | Arg types (in order) | Description |
| ------------- | ------------- | ------------- |
| LOAD_GENOME  | string  | Set the organism's genome to the given string and reset the organism |
| SET_REG  | char, val | Set the register (denoted by the char) to the given value |
| SET_IP | val | Set the instruction pointer to the given value |
| SET_RH | val | Set the read head to the given value |
| SET_WH | val | Set the write head to the given value |
| SET_FH | val | Set the flow head to the given value |
| EXECUTE | int | Execute the given number of instructions |
| CHECK_REG | char, val | Check if the register (denoted by the char) contains the given value, else case fails |
| CHECK_IP | val | Check if the instruction pointer equals the given value, else case fails |
| CHECK_RH | val | Check if the read head equals the given value, else case fails |
| CHECK_WH | val | Check if the write head equals the given value, else case fails |
| CHECK_FH | val | Check if the flow head equals the given value, else case fails |
| CHECK_STACK_A | int, val | Check if given position in stack A (0 = newest) equals the given value, else case fails |
| CHECK_STACK_B | int, val | Check if given position in stack B (0 = newest) equals the given value, else case fails |

## Interpreters

### Python
The python interpreter defines a `UAT_Interpreter_Base` class. 
To run uat files with your code, simply create a class that inherits from `UAT_Interpreter_Base`. 
Then you need to define one method for each keyword.
For keywords that are checking values, either `self.successful_checks` or `self.failed_checks` should be incremented depending on the outcome of the check.

The base class also defines a `load_file(filename)` method, that you can call on an instance of your derived class to load and run a uat file. Follow that with a call to the `print_results` method to see how many cases succeeded and failed.


Here's an example class that defines the `LOAD_GENOME` method:
``` 
class UAT_Interpreter_Example(UAT_Interpreter_Base):
  def __init__(self):
    UAT_Interpreter_Base.__init__(self)
    self.org = Organism()

  def load_genome(self, genome):
    self.org.genome = copy.deepcopy(genome)
    self.org.clear()
```

### C++
The C++ interpeter defines a `uat::UATInterpreter` class. 
To run uat files, simply create a class that inherits from `UATInterpreter`. (Note that it is templated on the register/head data type of you Avida hardware). 
`UATInterpreter` contains pure virtual methods that must be overridden in your derived class (one per keyword). If you miss any, the compiler will error. 
For keywords that are checking values, either `succesful_checks` or `failed_checks` should be incremented depending on the outcome.

To use your new derived class, simply create an instance (either passing a filename/istream to the constructor or to a call to `Load()`) and call the `Process()` method. 
To then view the number of successful and failed test cases, simply call the `PrintResults()` method.

*Note*: the C++ interpreter depends on Empirical (https://github.com/devosoft/Empirical/). In the future, this may be included as a submodule, but for now it must be added manually. It is recommended to build your Makefile off of an existing Empirical Makefile [(example)](https://github.com/devosoft/Empirical/blob/master/examples/base/Makefile). 

Here's an example derived class + method for `LOAD_GENOME`:
```
class UATInterpreter_Example : public uat::UATInterpreter<uint32_t>{
  ...
  public: 
  virtual void LoadGenome(const std::string& genome) override {
    // Logic to load your genome here!
  }
  ...
};
```
