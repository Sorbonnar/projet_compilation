// Test programme MiniC
int a = 0, b;
bool c = false;

void main() {
    int i, a = b, e = 2;
    bool d = true;

    if (c) {
        b;
    }
    for (i = 0; i < 10; i = i + 1) {
        a;
    }
    while (true) {
        a;
    }

    do {
        a;
    }
    while(d);

    a = b * e;
    a = b / e;
    a = b % e;
    a = b + e;
    a = b - e;
    c = b < a;
    c = b > a;
    c = b == a;
    c = b <= a;
    c = b >= a;
    c = b != a;
    c = d && c;
    c = b & a;
    c = d || c;
    c = b | a;
    c = b ^ a;
    c = b << a;
    c = b >> a;
    c = b >>> a;
    c = !a;
    c = ~d;

    print("a: ", a, "\n");
}