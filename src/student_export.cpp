#include <Rcpp.h>
#include "student.h"

//' Simulate a student
//'
//' @export
// [[Rcpp::export]]
std::vector<int> simulate_student() {
  Student s = Student("bob", 10, true);
  return s.GetFavoriteNumbers();
}

// Expose (some of) the Student class
RCPP_MODULE(RcppStudentEx){
  Rcpp::class_<Student>("Student")
  .constructor<std::string, int, bool>()
  .method("GetName", &Student::GetName)
  .method("GetAge", &Student::GetAge)
  .method("IsMale", &Student::IsMale)
  .method("GetFavoriteNumbers", &Student::GetFavoriteNumbers)
  .method("LikesBlue", &Student::LikesBlue);
}
