# Create a Student Object from the Student C++ Class

Allows for the creation of a Student Object in *C++* from *R* using the
*C++* Student class.

## Arguments

- name:

  Name of Student

- age:

  Age of Student

- male:

  Is Student a Male?

## Value

A `Student` object from the *C++* Student Class.

## Examples

``` r
##################
## Constructor

# Construct new Student object called "ben"
ben = new(Student, name = "Ben", age = 26, male = TRUE)

##################
## Getters

ben$LikesBlue()
#> [1] TRUE

ben$GetAge()
#> [1] 26

ben$IsMale()
#> [1] TRUE

ben$GetName()
#> [1] "Ben"

ben$GetFavoriteNumbers()
#> [1]  2  3  5  7 11
```
