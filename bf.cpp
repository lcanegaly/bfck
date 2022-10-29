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
    program_length_ = prog.length();
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
          pc_jmp_ = stack_[--sp_];
          program_[pc_].operand_ = pc_jmp_;
          program_[pc_jmp_].operand_ = pc_;
          break;
        default:
          std::cout << "unexpected input \n";
          return 0;
      }
      pc_++;
    }
  
    return 1;   
  }
  static int Excecute() {
    pc_ = 0;
    while (pc_ < program_length_){
      switch(program_[pc_].operator_){
        case increment_dp:
          dp_++;
          break;
        case decrement_dp:
          dp_--;
          break;
        case increment:
          data_[dp_]++;
          break;
        case decrement:
          data_[dp_]--;
          break;
        case output:
          std::cout << data_[dp_];
          break;
        case input:
          data_[dp_] = std::cin.get();
          break;
        case jump_forward:
          if (data_[dp_] == 0) {pc_ = program_[pc_].operand_ + 1; continue;} 
          break;
        case jump_backward:
          pc_ = program_[pc_].operand_;
          continue;
        default:
          std::cout << "unexpected instruction \n";
          return 0;
      }
      pc_++;
    }
    return 1;
  }

 private:
  static int pc_;
  static int pc_jmp_;
  static int dp_;
  static int sp_;
  static instruction program_[];
  static char stack_[];
  static char data_[];
  static int program_length_;
};

instruction Bfck::program_[1024];
char Bfck::stack_[64]{0};
char Bfck::data_[64]{0};
int Bfck::pc_ {0};
int Bfck::dp_ {0};
int Bfck::sp_ {0};
int Bfck::pc_jmp_ {0};
int Bfck::program_length_ {0};


int main(int, char**) {

  std::string test_program =
      "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>"
      ".<-.<.+++.------.--------.>>+.>++.";
  
  std::string test_program2 = "++[-]" ;
  
  //std::cout << "Program called: {## " << prog << " ##} \n";

  if (Bfck::Compile(test_program)) {
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
