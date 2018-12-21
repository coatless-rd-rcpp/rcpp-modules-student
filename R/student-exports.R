# Export the "Student" C++ class by explicitly requesting Student be
# exported via roxygen2's export tag.
#' @export Student

# Load the Rcpp module exposed with RCPP_MODULE( ... ) macro.
loadModule(module = "RcppStudentEx", TRUE)
