
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <functional>
#include "emp/io/File.hpp"
#include "emp/tools/string_utils.hpp"
#include "emp/datastructs/map_utils.hpp"
#include "emp/base/error.hpp"
#include "emp/base/vector.hpp"

namespace uat{

  template <typename DATA_T>
  class UATInterpreter{
  public:
    using data_t = DATA_T;
  private:
    emp::File file;
  protected:
    std::unordered_map<std::string, std::function<void(emp::vector<std::string>&)>> raw_function_map;
    size_t successful_checks;
    size_t failed_checks;
    size_t line_num;
    
    
    void FillRawFunctionMap(){
      raw_function_map["LOAD_GENOME"] = [this](emp::vector<std::string>& line_parts){
         LoadGenome_Raw(line_parts);
      };
      raw_function_map["SET_REG"] = [this](emp::vector<std::string>& line_parts){
         SetReg_Raw(line_parts);
      };
      raw_function_map["CHECK_REG"] = [this](emp::vector<std::string>& line_parts){
         CheckReg_Raw(line_parts);
      };
      raw_function_map["EXECUTE"] = [this](emp::vector<std::string>& line_parts){
         Execute_Raw(line_parts);
      };
      raw_function_map["SET_IP"] = [this](emp::vector<std::string>& line_parts){
         SetIP_Raw(line_parts);
      };
      raw_function_map["SET_RH"] = [this](emp::vector<std::string>& line_parts){
         SetRH_Raw(line_parts);
      };
      raw_function_map["SET_WH"] = [this](emp::vector<std::string>& line_parts){
         SetWH_Raw(line_parts);
      };
      raw_function_map["SET_FH"] = [this](emp::vector<std::string>& line_parts){
         SetFH_Raw(line_parts);
      };
      raw_function_map["CHECK_IP"] = [this](emp::vector<std::string>& line_parts){
         CheckIP_Raw(line_parts);
      };
      raw_function_map["CHECK_RH"] = [this](emp::vector<std::string>& line_parts){
         CheckRH_Raw(line_parts);
      };
      raw_function_map["CHECK_WH"] = [this](emp::vector<std::string>& line_parts){
         CheckWH_Raw(line_parts);
      };
      raw_function_map["CHECK_FH"] = [this](emp::vector<std::string>& line_parts){
         CheckFH_Raw(line_parts);
      };
      raw_function_map["CHECK_STACK_A"] = [this](emp::vector<std::string>& line_parts){
         CheckStackA_Raw(line_parts);
      };
      raw_function_map["CHECK_STACK_B"] = [this](emp::vector<std::string>& line_parts){
         CheckStackB_Raw(line_parts);
      };
    }
    
    void ThrowLineError(const std::string& error_str){
      std::cout << emp::ANSI_BrightRed() << "Error on line " << line_num 
          << emp::ANSI_Reset() << "!" << std::endl;
      emp_error(error_str);
    }

    void LoadGenome_Raw(emp::vector<std::string>& line_parts){
      if(line_parts.size() != 2){
        ThrowLineError("LOAD_GENOME expects exactly one argument (the genome to load)");
      }
      std::string& genome = line_parts[1];
      emp::justify(genome);
      emp::remove_chars(genome, "\"");
      this->LoadGenome(genome);
    }
    void SetReg_Raw(emp::vector<std::string>& line_parts){
      if(line_parts.size() != 3){
        ThrowLineError("SET_REG expects exactly two arguments (a character specifying the register and the value to set the register to)");
      }
      emp::remove_chars(line_parts[1], "'");
      char reg_char = line_parts[1][0];
      data_t val = emp::from_string<data_t>(line_parts[2]); 
      SetReg(reg_char, val);
    }
    void CheckReg_Raw(emp::vector<std::string>& line_parts){
      if(line_parts.size() != 3){
        ThrowLineError("CHECK_REG expects exactly two arguments (a character specifying the register and the value to check the register against)");
      }
      emp::remove_chars(line_parts[1], "'");
      char reg_char = line_parts[1][0];
      data_t val = emp::from_string<data_t>(line_parts[2]); 
      CheckReg(reg_char, val);
    }
    void Execute_Raw(emp::vector<std::string>& line_parts){
      if(line_parts.size() != 2){
        ThrowLineError("EXECUTE expects exactly one argument (the number of instructions to execute)");
      }
      size_t num_insts = emp::from_string<size_t>(line_parts[1]); 
      Execute(num_insts);
    }
    void SetIP_Raw(emp::vector<std::string>& line_parts){
      if(line_parts.size() != 2){
        ThrowLineError("SET_IP expects exactly one argument (the value to set the instruction pointer to)");
      }
      data_t val = emp::from_string<data_t>(line_parts[1]); 
      SetIP(val);
    }
    void SetRH_Raw(emp::vector<std::string>& line_parts){
      if(line_parts.size() != 2){
        ThrowLineError("SET_RH expects exactly one argument (the value to set the instruction pointer to)");
      }
      data_t val = emp::from_string<data_t>(line_parts[1]); 
      SetRH(val);
    }
    void SetWH_Raw(emp::vector<std::string>& line_parts){
      if(line_parts.size() != 2){
        ThrowLineError("SET_WH expects exactly one argument (the value to set the instruction pointer to)");
      }
      data_t val = emp::from_string<data_t>(line_parts[1]); 
      SetWH(val);
    }
    void SetFH_Raw(emp::vector<std::string>& line_parts){
      if(line_parts.size() != 2){
        ThrowLineError("SET_FH expects exactly one argument (the value to set the instruction pointer to)");
      }
      data_t val = emp::from_string<data_t>(line_parts[1]); 
      SetFH(val);
    }
    void CheckIP_Raw(emp::vector<std::string>& line_parts){
      if(line_parts.size() != 2){
        ThrowLineError("CHECK_IP expects exactly one argument (the value to check the instruction pointer against)");
      }
      data_t val = emp::from_string<data_t>(line_parts[1]); 
      CheckIP(val);
    }
    void CheckRH_Raw(emp::vector<std::string>& line_parts){
      if(line_parts.size() != 2){
        ThrowLineError("CHECK_RH expects exactly one argument (the value to check the instruction pointer against)");
      }
      data_t val = emp::from_string<data_t>(line_parts[1]); 
      CheckRH(val);
    }
    void CheckWH_Raw(emp::vector<std::string>& line_parts){
      if(line_parts.size() != 2){
        ThrowLineError("CHECK_WH expects exactly one argument (the value to check the instruction pointer against)");
      }
      data_t val = emp::from_string<data_t>(line_parts[1]); 
      CheckWH(val);
    }
    void CheckFH_Raw(emp::vector<std::string>& line_parts){
      if(line_parts.size() != 2){
        ThrowLineError("CHECK_FH expects exactly one argument (the value to check the instruction pointer against)");
      }
      data_t val = emp::from_string<data_t>(line_parts[1]); 
      CheckFH(val);
    }
    void CheckStackA_Raw(emp::vector<std::string>& line_parts){
      if(line_parts.size() != 3){
        ThrowLineError("CHECK_STACK_A expects exactly two arguments (the index of the value to check, and the value to check it against)");
      }
      size_t idx = emp::from_string<data_t>(line_parts[1]); 
      data_t val = emp::from_string<data_t>(line_parts[2]); 
      CheckStackA(idx, val);
    }
    void CheckStackB_Raw(emp::vector<std::string>& line_parts){
      if(line_parts.size() != 3){
        ThrowLineError("CHECK_STACK_B expects exactly two arguments (the index of the value to check, and the value to check it against)");
      }
      size_t idx = emp::from_string<data_t>(line_parts[1]); 
      data_t val = emp::from_string<data_t>(line_parts[2]); 
      CheckStackB(idx, val);
    }

  public: 
    UATInterpreter(){ 
      FillRawFunctionMap();
    }
    UATInterpreter(const std::string& filename){
      FillRawFunctionMap();
      Load(filename);
    }
    UATInterpreter(std::istream& is){
      FillRawFunctionMap();
      Load(is);
    }
    virtual ~UATInterpreter() { ; }
    

    void Load(std::istream& is){
      successful_checks = 0;
      failed_checks = 0;
      file.Load(is);
    }

    void Process(){
      file.RemoveComments("//");
      for(line_num = 1; line_num <= file.GetNumLines(); ++line_num){
        ProcessLine(file[line_num - 1]);
      }
    }

    void ProcessLine(std::string& line){
      emp::justify(line);
      if(line.size() == 0) return;
      emp::vector<std::string> line_parts = emp::slice(line, ' ');
      if(line_parts.size() < 2){
        ThrowLineError("Error! Invalid line! Expected at least two pieces of information");
      }
      if(!emp::Has(raw_function_map, line_parts[0])){
        emp_error("Error! No raw function for keyword \"" + line_parts[0] + "\"");
      }
      raw_function_map[line_parts[0]](line_parts);
    }

    void Load(const std::string& filename){
      std::ifstream ifs(filename, std::ios::in);
      Load(ifs);
    }

    void PrintResults(){
       std::cout << "##########################################" << std::endl;
       std::cout << "################ RESULTS #################" << std::endl;
       std::cout << "##########################################" << std::endl;
       std::cout << emp::ANSI_BrightGreen() << emp::ANSI_Bold() <<  
         "Successful checks: " << successful_checks << emp::ANSI_Reset() << std::endl;
       if(failed_checks == 0){
         std::cout << emp::ANSI_BrightGreen() << emp::ANSI_Bold() << 
           "Failed checks: " << failed_checks << emp::ANSI_Reset() << std::endl;
       }
       else{
         std::cout << emp::ANSI_BrightRed() << emp::ANSI_Bold() << 
           "Failed checks: " << failed_checks << emp::ANSI_Reset() << std::endl;
       }
    }
    
    virtual void LoadGenome(const std::string& /*genome*/) = 0; 
    virtual void SetReg(char /*reg_char*/, data_t /*val*/) = 0; 
    virtual void CheckReg(char /*reg_char*/, data_t /*val*/) = 0; 
    virtual void Execute(size_t /*num_inst*/) = 0; 
    virtual void SetIP(data_t /*val*/) = 0; 
    virtual void SetRH(data_t /*val*/) = 0; 
    virtual void SetWH(data_t /*val*/) = 0; 
    virtual void SetFH(data_t /*val*/) = 0; 
    virtual void CheckIP(data_t /*val*/) = 0; 
    virtual void CheckRH(data_t /*val*/) = 0; 
    virtual void CheckWH(data_t /*val*/) = 0; 
    virtual void CheckFH(data_t /*val*/) = 0; 
    virtual void CheckStackA(size_t /*idx*/, data_t /*val*/) = 0; 
    virtual void CheckStackB(size_t /*idx*/, data_t /*val*/) = 0; 

  };

}

