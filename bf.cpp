#include <iostream>
#include <string>

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
    for (int i = 0; i < prog.length(); i++){
      switch(prog[i]){
        case '>':
          program_[pc_].operator_= increment_dp;
          break;
        case '<':
          program_[pc_].operator_= decrement_dp;
          break;
        case '+':
          program_[pc_].operator_= increment;
          break;
        case '-':
          program_[pc_].operator_= decrement;
          break;
        case '.':
          program_[pc_].operator_= output;
          break;
        case ',':
          program_[pc_].operator_= input;
          break;
        case '[':
          program_[pc_].operator_= jump_forward;
          stack_[sp_++] = pc_;
          break;
        case ']':
          program_[pc_].operator_= jump_backward;
          pc_jmp_ = pc_;
          pc_ = stack_[--sp_];
          break;
        default:
          std::cout << "unexpected input \n";
          return 0;
      }
      pc_++;
    }
  
    return 1;   
  }
  static int Excecute() { return 1; }

 private:
  static int pc_;
  static int pc_jmp_;
  static int dp_;
  static int sp_;
  static instruction program_[];
  static char stack_[];
  static char data_[];
};

instruction Bfck::program_[1024];
char Bfck::stack_[64]{0};
char Bfck::data_[64]{0};
int Bfck::pc_ {0};
int Bfck::dp_ {0};
int Bfck::sp_ {0};
int Bfck::pc_jmp_ {0};


int main(int, char**) {

  std::string prog =
      "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>"
      ".<-.<.+++.------.--------.>>+.>++.";
  std::cout << "Program called: {## " << prog << " ##} \n";

  if (Bfck::Compile(prog)) {
    std::cout << "Compiled Successfully \n";
    if (Bfck::Excecute()) {
      std::cout << "Executed Successfully \n";
    } else {
      std::cout << "Error \n";
    }
  } else {
    std::cout << "Compile Error \n";
  }
}
