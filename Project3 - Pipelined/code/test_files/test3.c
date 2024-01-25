// test 3 : fibonacci code.
int main(){
	int num = 10;
	int result;
	result = fib(num);
    return result;
}

int fib(int n){
	int result;
	if (n <= 2){
        return 1;
    }
	else {
		return (fib(n-1) + fib(n-2));
	}
}