#include <iostream>
#include <string>
#include <fstream>

enum op{
  increment_dp,
  decrement_dp,
  increment,
  decrement,
  output,
  input,
  jump_forward,
  jump_backward
};

struct instruction {
  op operator_;
  char operand_;
};

class Bfck {
 public:
  static int Compile(std::string& prog) { 
    program_length_ = prog.length();
    for (int i = 0; i < prog.length(); i++){
      switch(prog[i]){
        case '>':
          program_[program_counter_].operator_= increment_dp;
          break;
        case '<':
          program_[program_counter_].operator_= decrement_dp;
          break;
        case '+':
          program_[program_counter_].operator_= increment;
          break;
        case '-':
          program_[program_counter_].operator_= decrement;
          break;
        case '.':
          program_[program_counter_].operator_= output;
          break;
        case ',':
          program_[program_counter_].operator_= input;
          break;
        case '[':
          program_[program_counter_].operator_= jump_forward;
          stack_[stack_pointer_++] = program_counter_;
          break;
        case ']':
          program_[program_counter_].operator_= jump_backward;
          program_counter_jmp_ = stack_[--stack_pointer_];
          program_[program_counter_].operand_ = program_counter_jmp_;
          program_[program_counter_jmp_].operand_ = program_counter_;
          break;
        //default:
          //std::cout << "unexpected input: \n" << prog[i];     
          //return 0;
      }
      program_counter_++;
    }
  
    return 1;   
  }
  static int Excecute() {
    program_counter_ = 0;
    while (program_counter_ < program_length_){
      Monitor(0);
      switch(program_[program_counter_].operator_){
        case increment_dp:
          data_pointer_++;
          break;
        case decrement_dp:
          data_pointer_--;
          break;
        case increment:
          data_[data_pointer_]++;
          break;
        case decrement:
          data_[data_pointer_]--;
          break;
        case output:
          std::cout << data_[data_pointer_];
          break;
        case input:
          data_[data_pointer_] = std::cin.get();
          break;
        case jump_forward:
          if (data_[data_pointer_] == 0) {program_counter_ = program_[program_counter_].operand_ + 1; continue;} 
          break;
        case jump_backward:
          program_counter_ = program_[program_counter_].operand_;
          continue;
        default:
          std::cout << "unexpected instruction \n";
          return 0;
      }
      program_counter_++;
    }
    return 1;
  }

  static void Monitor(int mode) {
    static char* ins = "+-><.,[]";
    std::cout << "INS{" << ins[program_[program_counter_].operator_]
              << "}  MEM{";

    for (int i = 0; i < 30; i++) 
      if (i == data_pointer_)
        std::cout << "| ->" << (int)data_[i] << "<- ";
      else
        std::cout << "|" << (int)data_[i];    
    std::cout << "}\r" << std::flush;

    if (mode == 1) std::cin.get();
  }

 private:
  static int program_counter_;
  static int program_counter_jmp_;
  static int data_pointer_;
  static int stack_pointer_;
  static instruction program_[];
  static char stack_[];
  static char data_[];
  static int program_length_;
};

instruction Bfck::program_[1024];
char Bfck::stack_[64]{0};
char Bfck::data_[64]{0};
int Bfck::program_counter_ {0};
int Bfck::data_pointer_ {0};
int Bfck::stack_pointer_ {0};
int Bfck::program_counter_jmp_ {0};
int Bfck::program_length_ {0};


int main(int argc, char** argv) {

  if (argc < 2) return 0;
  
  std::string flag = argv[1];
  std::string input = argv[2];
  
  if (flag == "-r"){
    Bfck::Compile(input);
    Bfck::Excecute();
  }else if (flag == "-f"){
    std::string program_source;
    std::ifstream source_file (input);
    if (source_file.is_open()) {
      std::string line;
      while ( getline (source_file,line) )
        {
          program_source += line;
        }
    }
    source_file.close();
    Bfck::Compile(program_source);
    Bfck::Excecute();
  }else {
    std::cout << "please specify a file, or string";
  }

  //test hello world program
  // "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.";
}
