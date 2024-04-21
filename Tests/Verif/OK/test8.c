// Expected output:
// Addition: 8
// Subtraction: 4
// Multiplication: 12
// Division 1: 0
// Division 2: 3
// Modulo: 2
//

void main() {
    int a = 6;
    int b = 2;
    
    int add = a + b;
    int sub = a - b;
    int mul = a * b;
    int div = b / a;
    int div2 = a / b;
    int mod = b % a;

    print("Addition: ", add);
    print("\nSubtraction: ", sub);
    print("\nMultiplication: ", mul);
    print("\nDivision 1: ", div);
    print("\nDivision 2: ", div2);
    print("\nModulo: ", mod);
    print("\n");
}
