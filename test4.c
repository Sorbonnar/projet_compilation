int intos = 10;
bool boolustre = false;

void main() {
    int a = 5;
    int b = intos;
    int i;
    bool c = true;
    
    // Arithmetic operations
    int sum = a + b;
    int diff = a - b;
    int product = a * b;
    int quotient = b / a;
    int remainder = b % a;

    // Bitwise operations (on int values)
    int bitwiseAnd = a & b;
    int bitwiseOr = a | b;
    int bitwiseXor = a ^ b;

    // Unary operations
    bool notC = !c;
    int negA = -a;
    int bitwiseNotA = ~a;

    // Bit shifts
    int shiftLeft = a << 2;
    int shiftRight = b >> 2;

    // Print results of arithmetic operations
    print("Sum: ", sum);
    print("Difference: ", diff);
    print("Product: ", product);
    print("Quotient: ", quotient);
    print("Remainder: ", remainder);
    
    // Logical operations
    if (c) {
        print("Logical True");
    } else {
        print("Logical False");
    }
    
    // Comparison operations
    if (a == b) {
        print("a equals b");
    } else if (a != b) {
        print("a not equal b");
    }

    if (a < b) {
        print("a is less than b");
    } else if (a > b) {
        print("a is greater than b");
    }

    if (a <= b) {
        print("a is less than or equal to b");
    } else if (a >= b) {
        print("a is greater than or equal to b");
    }

    // Print results of bitwise operations
    print("Bitwise AND: ", bitwiseAnd);
    print("Bitwise OR: ", bitwiseOr);
    print("Bitwise XOR: ", bitwiseXor);

    // Print results of unary operations
    print("Not C: ", notC);
    print("Negative A: ", negA);
    print("Bitwise Not A: ", bitwiseNotA);

    // Print results of bit shifts
    print("Shift Left: ", shiftLeft);
    print("Shift Right: ", shiftRight);

    // Control structures
    while (a < 8) {
        print("a in while: ", a);
        a = a + 1;
    }

    for (i = 0; i < 5; i = i + 1) {
        print("i in for: ", i);
    }

    do {
        b = b - 1;
        print("b in do-while: ", b);
    } while (b > 5);
}
