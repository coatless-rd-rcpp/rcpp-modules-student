#' @details
#' RcppStudent is a package showing how to use Rcpp Modules to make available
#' a C++ class under S4.
#'
#' @useDynLib RcppStudent, .registration = TRUE
#' @import methods Rcpp
"_PACKAGE"


# Export the "Student" C++ class by explicitly requesting Student be
# exported via roxygen2's export tag.
#' @export Student

loadModule(module = "RcppStudentEx", TRUE)

