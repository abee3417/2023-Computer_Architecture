// test 2 : simple jump test code.
int foo(){
    int a = 100;
    int b;
    int c;
    b = bar(a);
    c = a + b;
    return c;
}

int bar(int arg){
    int res = arg + 1;
    return res;
}