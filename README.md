## Exposing _C++_ Classes Through Rcpp Modules

[![Travis-CI Build Status](https://travis-ci.org/r-pkg-examples/rcpp-modules.svg?branch=master)](https://travis-ci.org/r-pkg-examples/rcpp-modules)

The `RcppStudent` _R_ package provides an example of using [Rcpp Modules](https://cran.r-project.org/web/packages/Rcpp/vignettes/Rcpp-modules.pdf)
to expose C++ functions and classes to _R_.

Code for this example is based a question that arose on StackOverflow titled
"[Expose simple C++ Student class to R using Rcpp modules](https://stackoverflow.com/questions/53659500/expose-simple-c-student-class-to-r-using-rcpp-modules)" 
by [Ben Gorman](https://github.com/ben519).

### Usage

To install the package, you must first have a compiler on your system that is
compatible with R. For help on obtaining a compiler consult either
[macOS](http://thecoatlessprofessor.com/programming/r-compiler-tools-for-rcpp-on-os-x/)
or
[Windows](http://thecoatlessprofessor.com/programming/rcpp/install-rtools-for-rcpp/)
guides.

With a compiler in hand, one can then install the package from GitHub by:

```r
# install.packages("devtools")
devtools::install_github("r-pkg-examples/rcpp-modules")
library("RcppStudent")
```

### Implementation Details

This guide focuses on providing an implementation along the path suggested
in [Rcpp Modules Section 2.2: Exposing C++ classes using Rcpp modules](https://cran.r-project.org/web/packages/Rcpp/vignettes/Rcpp-modules.pdf#page=4).
In particular, the focus is to expose a pure _C++_ class inside of _R_ without
modifying the underlying _C++_ class. Largely, this means that the _C++_ class
must be "marked up" for export using `RCPP_MODULE( ... )` macro in a separate
file.

### C++ Class Definition

Inside of [src/student.h](src/student.h), the **definition** of the _C++_ class
is written with an inclusion guard. The definition is a "bare-bones" overview
of what to expect. The meat or the implementation of the class is given in the
[src/student.cpp](src/student.cpp) file.

```c++
//  Student.h

#ifndef Student_H
#define Student_H

#include <string>
#include <vector>

class Student
{
public:

  // Constructor
  Student(std::string name, int age, bool male);

  // Getters
  std::string GetName();
  int GetAge();
  bool IsMale();
  std::vector<int> GetFavoriteNumbers();

  // Methods
  bool LikesBlue();

private:

  // Member variables
  std::string name;
  int age;
  bool male;
  std::vector<int> favoriteNumbers;
};

#endif /* Student_H */
```

#### C++ Class Implementation

In [src/student.cpp](src/student.cpp), we implement the **meat** behind the _C++_ class. 

```cpp
//  Student.cpp

#include "student.h"

// Constructor
Student::Student(std::string name, int age, bool male) {
  this->name = name;
  this->age = age;
  this->male = male;
  this->favoriteNumbers = {2, 3, 5, 7, 11};
}

// Getters
bool Student::IsMale() { return male; }
int Student::GetAge() { return age; }
std::string Student::GetName() { return name; }
std::vector<int> Student::GetFavoriteNumbers() { return favoriteNumbers; }

// Methods
bool Student::LikesBlue() {
  return (male || age >= 10);
}
```

### Writing the Glue

With the class definition and implementation in hand, the task switches to exposing
the definition into _R_ by creating [src/student_export.cpp](src/student_export.cpp).
Within this file, the Rcpp Module is defined using the `RCPP_MODULE( ... )` macro.
Through the macro, the class' information must be specified using different
member functions of `Rcpp::class_`. A subset of these member functions is
given next:

- Constructor: 
     - `.constructor<PARAMTYPE1, PARAMTYPE2>()`
         - Exposes a constructor with recognizable _C++_ data types.
         - e.g. `double`, `int`, `std::string`, and so on.
- Methods: 
     - `.method("FunctionName", &ClassName::FunctionName)`
         - Exposes a class method from `ClassName` given by `FunctionName`.
     - `.property("VariableName", &ClassName::GetFunction, &ClassName::SetFunction )`
         - Indirect access to a Class' fields through getter and setter functions.
- Fields: 
     - `.field("VariableName", &ClassName::VariableName, "documentation for VariableName")`
         - Exposes a public field with **read and write** access from R
     - `.field_readonly("VariableName", &Foo::VariableName, "documentation for VariableName read only")`
         - Exposes a public field with read access from R


```c++
// student_export.cpp

// Include Rcpp system header file (e.g. <>)
#include <Rcpp.h>

// Include our definition of the student file (e.g. "")
#include "student.h"

// Expose (some of) the Student class
RCPP_MODULE(RcppStudentEx) {             // Name used to "loadModule" in R script
  Rcpp::class_<Student>("Student")       // This must be the C++ class name.
  .constructor<std::string, int, bool>()
  .method("GetName", &Student::GetName)
  .method("GetAge", &Student::GetAge)
  .method("IsMale", &Student::IsMale)
  .method("GetFavoriteNumbers", &Student::GetFavoriteNumbers)
  .method("LikesBlue", &Student::LikesBlue);
}
```

### Exporting and documenting an Rcpp Module

In the [R/student-exports.R](R/student-exports.R) file, write the load 
statement for the module exposed via the `RCPP_MODULE( ... )` macro. In addition,
make sure to export the class name so that when the package is loaded anyone 
can access it via `new()`.

```r
# Export the "Student" C++ class by explicitly requesting Student be
# exported via roxygen2's export tag.
#' @export Student

loadModule(module = "RcppStudentEx", TRUE)
```

### Package Structure

To register the required components for Rcpp Modules, the `NAMESPACE` file 
must be populated with imports for `Rcpp` and the `methods` _R_ packages.
In addition, the package's dynamic library must be specified as well.

There are two ways to go about this:

1. Let `roxygen2` automatically generate the `NAMESPACE` file; or
2. Manually specify in the `NAMESPACE` file.

The `roxygen2` markup required can be found in [R/student-pkg.R](R/student-pkg.R).

```r
#' @useDynLib RcppStudent, .registration = TRUE
#' @import methods Rcpp
"_PACKAGE"
```

Once the above is run during the documentation generation phase, the
[`NAMESPACE`](NAMESPACE) file will be created with:

```r
# Generated by roxygen2: do not edit by hand

export(Student)
import(Rcpp)
import(methods)
useDynLib(RcppStudent, .registration = TRUE)
```

Make sure to build and reload the package prior to accessing methods.

### Calling an Rcpp Module in R

At this point, everything boils down to constructing an object from the class
using `new()` from the `methods` package. The `new()` function initializes a
_C++_ object from the specified _C++_ class and treats it like a 
traditional **S4** object.

```r
##################
## Constructor

# Construct new Student object called "ben"
ben = new(Student, name = "Ben", age = 26, male = TRUE)

##################
## Getters

ben$LikesBlue()
# [1] TRUE

ben$GetAge()
# [1] 26

ben$IsMale()
# [1] TRUE

ben$GetName()
# [1] "Ben"

ben$GetFavoriteNumbers()
# [1]  2  3  5  7 11
```

## License

GPL (\>= 2)
