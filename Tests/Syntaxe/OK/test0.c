int int1 = 20;
bool bool1 = false;

void main() {
    int a = 5;
    int b = int1;
    bool c = true;
    int i;
    
    int add = a + b;
    int sub = a - b;
    int mul = a * b;
    int div = b / a;
    int mod = b % a;

    int band = a & b;
    int bor = a | b;
    int bxor = a ^ b;

    bool notC = !c;
    int minA = -a;
    int notA = ~a;

    int SL = a << 2;
    int SR = b >> 2;

    print("Addition: ", add);
    print("Subtraction: ", sub);
    print("Multiplication: ", mul);
    print("Division: ", div);
    print("Modulo: ", mod);
    
    if (c) {
        print("True");
    }
    else {
        print("False");
    }
    
    if (a == b) {
        print("a == b");
    }
    else if (a != b) {
        print("a != b");
    }

    if (a < b) {
        print("a < b");
    }
    else if (a > b) {
        print("a > b");
    }

    if (a <= b) {
        print("a <= b");
    }
    else if (a >= b) {
        print("a >= b");
    }

    print("BAND: ", band);
    print("BOR: ", bor);
    print("BXOR: ", bxor);

    print("!C: ", notC);
    print("-A: ", minA);
    print("~A: ", notA);

    print("Shift Left: ", SL);
    print("Shift Right: ", SR);

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

    print("End of main");
}
