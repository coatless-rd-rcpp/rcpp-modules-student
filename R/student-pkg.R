#' @details
#' RcppStudent is a package showing how to use Rcpp Modules to make available
#' a C++ class under S4.
#'
#' @useDynLib RcppStudent, .registration = TRUE
#' @import methods Rcpp
#' @exportPattern "^[[:alpha:]]+"
"_PACKAGE"

loadModule(module = "RcppStudentEx", TRUE)
