# Big Number c/c++

This project is to implement Big number library for c/c++ with all kind of arithmetic operation + / * -
The library is simple, no optimization is done in algorithms.
most algorithms are based on divide and concer.
### Installing
For Pc : 
1 - download lib
2 - Do "make"
For arduino : 
1 - Download lib.
2 - Do "make arduino"
3 - add lib to extern libraries (Arduino Ide)

```
Give the example
```
 Bn a;
 Bn b;
 Bn res;
 parseStrToBN(&b,"12344353464562453245345.12323143241341341234132414");
 parseStrToBN(&a,"3464356347523452314145426345635634563.464563457324134123412341320201032412304");
 
 uadd(a,b,&c); // unsigned addition
 add(a,b,&c); // sign addition
 umul(a,b,&c); // unsigned multiplication
 mul(a,b,&c); // signed multiplication


 
```
until finished
```

End with an example of getting some data out of the system or using it for a little demo

## Running the tests

Explain how to run the automated tests for this system

### Break down into end to end tests

Explain what these tests test and why

```
Give an example
```

```
Give an example
```

## Deployment

Add additional notes about how to deploy this on a live system

## Built With

* Make,GCC 



## Authors

* **Seyyidahmed Lahmer**


## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details



