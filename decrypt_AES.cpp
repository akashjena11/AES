//every function till extended gcd has been designed keeping in mind 9-bit representations.

//Let F be the given field.

//sub_bytes takes an 8-bit input and converts it into its 9-bit representation and uses the auxiliary procedures and obtains a 9-bit string. it convrts the 9-bit string to 8-bit representation and outputs the 8-bit string.

#include<iostream>
using namespace std;

int *field_add(int *a, int *b)    //adds 2 9-bit strings in the field
{
    int i;
    int *c;
    c=new int [9];
    for(i=0;i<9;i++)
    {
        c[i]=(a[i]+b[i])%2;
    }
    return c;
}

int *power_to_string(int n)  //input=n. output = 9-bit representation of x^n.
{
    int *b,*c,d[9],i;
    b=new int [9];
    c=new int [9];

    if(n<9)
    {
        for(i=0;i<9;i++)
        {
            b[i]=0;
        }
        b[n]=1;
        return b;
    }

    if(n==9)
    {
        b[0]=0; b[2]=1;
        b[1]=1; b[5]=1;
        b[3]=0; b[6]=0;
        b[4]=1; b[7]=0; b[8]=0;
    }
    if(n==9) return b;

    c=power_to_string(n-1);

    if(c[8]==1)
    {
        for(i=8;i>0;i--)
        {
            c[i]=c[i-1];
        }
        c[0]=0;

        b=field_add(c,power_to_string(9));
        return b;
    }

    else
    {
        for(i=8;i>0;i--)
        {
            c[i]=c[i-1];
        }
        c[0]=0;
        return c;
    }
}

int *field_mult(int *a, int *b)  //multiplies two field elements a and b.
{
    int i,j;
    int *c,aa[17],bb[17],*d;
    c=new int [9];
    d=new int [17];

    for(i=0;i<17;i++)
    {
        d[i]=0;
        if(i<9) { aa[i]=a[i]; bb[i]=b[i]; }
        else { aa[i]=0; bb[i]=0; }
    }

    for(i=0;i<17;i++)
    {
        for(j=0;j<i+1;j++)
        {
            d[i]=d[i]+(aa[j]*bb[i-j]);
            d[i]=d[i]%2;
        }
    }

    for(i=0;i<9;i++)
    {
        c[i]=d[i];
    }

    for(i=9;i<17;i++)
    {
        if(d[i]) c=field_add(c,power_to_string(i));
    }

    return c;
}

int degree(int *a)            //finds the degree of a polynomial(field element).
{
    int i;
    for(i=8;i>-1;i--)
    {
        if(a[i]) return i;
    }
}

int zero_checker(int *a)      //checks if a string is zero or not.
{
    int m=1,i;
    for(i=0;i<9;i++)
    {
        if(a[i]==1) {m=0; break;}
    }
    return m;
}

int *quotient(int *J, int *K)   // divides the string J by the string K and outputs the quotient.
{
    int *L,i;
    L=new int [9];

    for(i=0;i<9;i++)
    {
        L[i]=0;
    }

    while(degree(J)>=degree(K))
    {
        L=field_add(power_to_string(degree(J)-degree(K)),L);
        J=field_add(J,field_mult(L,K));
        if(zero_checker(J)) return L;
    }

    return L;
}

int *Remainder(int *J, int *K)          // divides the string J by the string K and outputs the remainder.
{
    int *L,i;
    L=new int [9];

    for(i=0;i<9;i++)
    {
        L[i]=0;
    }

    while(degree(J)>=degree(K))
    {
        L=field_add(power_to_string(degree(J)-degree(K)),L);
        J=field_add(J,field_mult(L,K));
        if(zero_checker(J)) return J;
    }

    return J;
}

int *extended_gcd(int *a, int *b)  //Assume d=(a,b) and d=ax+by. The first 9 bits of d_x_y contain d(d_x_y[0-8]=d), the next 9 bits contain x(d_x_y[9-17]=x) and the next 9 bits contain y(d_x_y[18-27]=y).
{
    int i;
    int *d_x_y;
    d_x_y=new int [27];

    if(zero_checker(b))
    {
        for(i=0;i<9;i++)
        {
            d_x_y[i]=a[i];
            d_x_y[i+9]=0;
            d_x_y[i+18]=0;
        }
        d_x_y[9]=1;

        return d_x_y;
    }

    int *D_X_Y;
    D_X_Y=new int [27];
    D_X_Y=extended_gcd(b,Remainder(a,b));

    int *xx,*yy,*z;
    xx=new int [9];
    yy=new int [9];
    z=new int [9];

    for(i=0;i<9;i++)
    {
        xx[i]=D_X_Y[i+9];
        yy[i]=D_X_Y[i+18];
    }

    z=field_add(xx,field_mult(yy,quotient(a,b)));
    for(i=0;i<9;i++)
    {
        d_x_y[i]=D_X_Y[i];
        d_x_y[i+9]=yy[i];
        d_x_y[i+18]=z[i];
    }

    return d_x_y;
}

int *field_inv(int *a)                  //finds the inverse of the element a.
{
    int *ONE,*d_x_y,*inverse,i;
    ONE=new int [9];
    inverse=new int [9];
    d_x_y=new int [27];

    ONE[0]=1; ONE[1]=1; ONE[2]=0;
    ONE[3]=1; ONE[4]=1; ONE[5]=0;
    ONE[6]=0; ONE[7]=0; ONE[8]=1;

    d_x_y=extended_gcd(ONE,a);

    for(i=0;i<9;i++)
    {
        inverse[i]=d_x_y[18+i];
    }

    return inverse;
}

int *eight_to_nine(int *a)
{
    int *A,i;
    A=new int [9];

    for(i=0;i<8;i++)
    {
        A[i]=a[i];
    }
    A[8]=0;

    return A;
}

int *nine_to_eight(int *A)    //this may not work for 8-degree polynomials. but thats not a problem. because in our implementation of AES, field_inv will never output an 8-degree polynomial.
{
    int *a,i;
    a=new int [8];
    if(A[8]==0)
    {
        for(i=0;i<8;i++)
            a[i]=A[i];

        return a;
    }

    int *b;
    b=new int [8];

    for(i=0;i<8;i++)
        a[i]=A[i];

    b[0]=1; b[1]=1; b[2]=0; b[3]=1; b[4]=1; b[5]=0; b[6]=0; b[7]=0;

    for(i=0;i<8;i++)
        a[i]=(a[i]+b[i])%2;

    return a;
}

int *sub_bytes(int *a)
{
    int *A,i,*c,*b;
    A=new int [9];
    c=new int [8];
    b=new int [8];

    A=eight_to_nine(a);

    if(!(zero_checker(A))) A=field_inv(A);

    a=nine_to_eight(A);

    c[0]=1; c[1]=1; c[2]=0; c[3]=0;
    c[4]=0; c[5]=1; c[6]=1; c[7]=0;

    for(i=0;i<8;i++)
    {
        b[i]=(a[i]+a[(i+4)%8]+a[(i+5)%8]+a[(i+6)%8]+a[(i+7)%8]+c[i])%2;
    }

    return b;
}

int *subbytes_inv(int *b)
{
    int i;
    int *c,*C,*a,*A,*d,*D,*B;

    c=new int [8]; C=new int [9];
    a=new int [8]; A=new int [9];
    d=new int [8]; D=new int [9]; B=new int [9];

    c[0]=1; c[1]=1; c[2]=0; c[3]=0;
    c[4]=0; c[5]=1; c[6]=1; c[7]=0;

    C=eight_to_nine(c);
    B=eight_to_nine(b);

    D=field_add(B,C);

    d=nine_to_eight(D);

    for(i=0;i<8;i++)
        a[i]=(d[(i+2)%8]+d[(i+5)%8]+d[(i+7)%8])%2;

    A=eight_to_nine(a);

    A=field_inv(A);

    a=nine_to_eight(A);

    return a;
}

int ***subbytes_inv_on_state(int ***state)
{
    int i,j;

    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
            state[i][j]=subbytes_inv(state[i][j]);
    }

    return state;
}

int ***mixcolm_inv(int ***s, int n)
{
    int i;
    int *x0,*x1,*x2,*x3,*t0,*t1,*t2,*t3;

    x0=new int[9]; x1=new int[9]; x2=new int[9]; x3=new int[9];
    t0=new int[9]; t1=new int[9]; t2=new int[9]; t3=new int[9];

    t0=eight_to_nine(s[0][n]);
    t1=eight_to_nine(s[1][n]);
    t2=eight_to_nine(s[2][n]);
    t3=eight_to_nine(s[3][n]);

    for(i=0;i<9;i++)
    {
        x0[i]=0; x1[i]=0; x2[i]=0; x3[i]=0;
    }

    x0[1]=1; x0[2]=1; x0[3]=1;
    x1[0]=1; x1[1]=1; x1[3]=1;
    x2[0]=1; x2[2]=1; x2[3]=1;
    x3[0]=1; x3[3]=1;

    s[0][n]=nine_to_eight(field_add(field_mult(x0,t0),field_add(field_mult(x1,t1),field_add(field_mult(x2,t2),field_mult(x3,t3)))));
    s[1][n]=nine_to_eight(field_add(field_mult(x3,t0),field_add(field_mult(x0,t1),field_add(field_mult(x1,t2),field_mult(x2,t3)))));
    s[2][n]=nine_to_eight(field_add(field_mult(x2,t0),field_add(field_mult(x3,t1),field_add(field_mult(x0,t2),field_mult(x1,t3)))));
    s[3][n]=nine_to_eight(field_add(field_mult(x1,t0),field_add(field_mult(x2,t1),field_add(field_mult(x3,t2),field_mult(x0,t3)))));

    return s;
}

int ***mixcolm_inv_on_state(int ***state)
{
    int i;

    for(i=0;i<4;i++)
        state=mixcolm_inv(state,i);

    return state;
}

int *equate(int *a)
{
    return a;
}

int ***shiftrows_inv(int ***s)
{
    int *x;
    x=new int [8];

    x=equate(s[1][3]);
    s[1][3]=equate(s[1][2]);
    s[1][2]=equate(s[1][1]);
    s[1][1]=equate(s[1][0]);
    s[1][0]=equate(x);

    x=equate(s[2][2]);
    s[2][2]=equate(s[2][0]);
    s[2][0]=equate(x);
    x=equate(s[2][3]);
    s[2][3]=equate(s[2][1]);
    s[2][1]=equate(x);

    x=equate(s[3][0]);
    s[3][0]=equate(s[3][1]);
    s[3][1]=equate(s[3][2]);
    s[3][2]=equate(s[3][3]);
    s[3][3]=equate(x);

    return s;
}

int *word_add(int *a, int *b)    //adds 2 32-bit strings.
{
    int *c,i;
    c=new int[32];

    for(i=0;i<32;i++)
        c[i]=(a[i]+b[i])%2;

    return c;
}

int *copy_L2s(int *a, int j)  //takes input a 32-bit string and an index j. copies a[j to j+7] to x[8]. outputs x.
{
    int *x,i;
    x=new int [8];

    for(i=0;i<8;i++)
        x[i]=a[j+i];

    return x;
}

void copy_s2L(int *x,int j,int *a)  //x is 8-bit. a is 32-bit. this function copies x to a[j to j+7].
{
    int i;
    for(i=0;i<8;i++)
        a[j+i]=x[i];
}

int *rot_word(int *a)
{
    int *x,*y;
    x=new int [8];
    y=new int [8];

    y=copy_L2s(a,0);
    x=copy_L2s(a,8);
    copy_s2L(x,0,a);
    x=copy_L2s(a,16);
    copy_s2L(x,8,a);
    x=copy_L2s(a,24);
    copy_s2L(x,16,a);
    copy_s2L(y,24,a);

    return a;
}

int *sub_word(int *a)
{
    int *x;
    x=new int [8];

    x=copy_L2s(a,0); x=sub_bytes(x); copy_s2L(x,0,a);
    x=copy_L2s(a,8); x=sub_bytes(x); copy_s2L(x,8,a);
    x=copy_L2s(a,16); x=sub_bytes(x); copy_s2L(x,16,a);
    x=copy_L2s(a,24); x=sub_bytes(x); copy_s2L(x,24,a);

    return a;
}

int **key_expansion(int **key) //key is a string of bytes of length 16.
{
    int **Rcon,i,j;
    Rcon=new int *[11];

    for(i=0;i<11;i++)
        Rcon[i]=new int [32];

    for(i=1;i<11;i++)
    {
        for(j=0;j<32;j++)
            Rcon[i][j]=0;
    }

    Rcon[1][7]=1; Rcon[2][6]=1; Rcon[3][5]=1; Rcon[4][4]=1;
    Rcon[5][3]=1; Rcon[6][2]=1; Rcon[7][1]=1; Rcon[8][0]=1;
    Rcon[9][3]=1; Rcon[9][4]=1; Rcon[9][6]=1; Rcon[9][7]=1;
    Rcon[10][2]=1; Rcon[10][3]=1; Rcon[10][5]=1; Rcon[10][6]=1;

    int **w,*x;
    w=new int *[44];
    x=new int [8];

    for(i=0;i<44;i++)
        w[i]=new int [32];

    for(i=0;i<4;i++)
    {
        x=equate(key[4*i]);
        copy_s2L(x,0,w[i]);
        x=equate(key[(4*i)+1]);
        copy_s2L(x,8,w[i]);
        x=equate(key[(4*i)+2]);
        copy_s2L(x,16,w[i]);
        x=equate(key[(4*i)+3]);
        copy_s2L(x,24,w[i]);
    }

    int *temp;
    temp=new int [32];

    for(i=4;i<44;i++)
    {
        temp=equate(w[i-1]);
        if (i%4) temp=word_add(Rcon[i/4],sub_word(rot_word(temp)));
        w[i]=word_add(w[i-4],temp);
    }

    return w;
}

int ***add_subkey(int ***state, int *x, int j)   //x=32-bit. this function xors the j-th row of state with x.
{
    int *s_word,i;
    s_word=new int [32];

    for(i=0;i<4;i++)
        copy_s2L(state[j][i],8*i,s_word);

    s_word=word_add(s_word,x);

    for(i=0;i<4;i++)
        state[j][i]=copy_L2s(s_word,8*i);

    return state;
}

int ***Add_roundkey(int ***state, int **w, int j)   //w=expanded key(see STINSON). j=j-th round.
{
    int i;

    for(i=0;i<4;i++)
        state=add_subkey(state,w[j+i],i);

    return state;
}

int*** AES_decrypt(int ***state, int **key)
{
    int **w,i;
    w=new int *[44];

    for(i=0;i<44;i++)
        w[i]=new int [8];

    w=key_expansion(key);

    state=Add_roundkey(state,w,10);
    state=shiftrows_inv(state);
    state=subbytes_inv_on_state(state);      //inverse of step-3 completed.

    for(i=9;i>0;i--)
    {
        state=Add_roundkey(state,w,i);
        state=mixcolm_inv_on_state(state);
        state=shiftrows_inv(state);
        state=subbytes_inv_on_state(state);
    }                                               //inverse of step2 completed.

    state=Add_roundkey(state,w,0);                  //inverse of step1 completed.

    return state;
}

int main()
{
    int i,j,k;

    int ***state;
    state=new int**[4];

    for(i=0;i<4;i++)
        state[i]=new int*[4];

    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
            state[i][j]=new int[8];
    }

    cout<<"\nenter the elements of state\n\n";

    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
        {
            for(k=0;k<8;k++)
                cin>>state[i][j][k];
        }
    }

    int **key;
    key=new int*[16];

    for(i=0;i<16;i++)
        key[i]=new int[8];

    cout<<"\nkey = \n\n";
    for(i=0;i<16;i++)
    {
        for(j=0;j<8;j++)
            cin>>key[i][j];
    }

    cout<<"\nstate = \n\n";
    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
        {
            for(k=0;k<8;k++)
            {
                cout<<state[i][j][k];
            }
            cout<<"  ";
        }
        cout<<"\n\n";
    }

    cout<<"\nkey = \n\n";
    for(i=0;i<16;i++)
    {
        for(j=0;j<8;j++)
            cout<<key[i][j];
        cout<<" ";
    }

    state=AES_decrypt(state,key);

    cout<<"\nD(state) = \n\n";
    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
        {
            for(k=0;k<8;k++)
            {
                cout<<state[i][j][k];
            }
            cout<<"  ";
        }
        cout<<"\n\n";
    }
}
