#include<iostream>
using namespace std;

int *f_add(int *a, int *b)
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

int *p2s(int n)
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

    c=p2s(n-1);

    if(c[8]==1)
    {
        for(i=8;i>0;i--)
        {
            c[i]=c[i-1];
        }
        c[0]=0;

        b=f_add(c,p2s(9));
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

int *f_mult(int *a, int *b)
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
        if(d[i]) c=f_add(c,p2s(i));
    }

    return c;
}

int deg(int *a)
{
    int i;
    for(i=8;i>-1;i--)
    {
        if(a[i]) return i;
    }
}

int is_it_0(int *a)
{
    int m=1,i;
    for(i=0;i<9;i++)
    {
        if(a[i]==1) {m=0; break;}
    }
    return m;
}

int *VF(int *J, int *K)
{
    int *L,i;
    L=new int [9];

    for(i=0;i<9;i++)
    {
        L[i]=0;
    }

    while(deg(J)>=deg(K))
    {
        L=f_add(p2s(deg(J)-deg(K)),L);
        J=f_add(J,f_mult(L,K));
        if(is_it_0(J)) return L;
    }

    return L;
}

int *VS(int *J, int *K)
{
    int *L,i;
    L=new int [9];

    for(i=0;i<9;i++)
    {
        L[i]=0;
    }

    while(deg(J)>=deg(K))
    {
        L=f_add(p2s(deg(J)-deg(K)),L);
        J=f_add(J,f_mult(L,K));
        if(is_it_0(J)) return J;
    }

    return J;
}

int *aux_inv(int *a, int *b)
{
    int i;
    int *d_x_y;
    d_x_y=new int [27];

    if(is_it_0(b))
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
    D_X_Y=aux_inv(b,VS(a,b));

    int *xx,*yy,*z;
    xx=new int [9];
    yy=new int [9];
    z=new int [9];

    for(i=0;i<9;i++)
    {
        xx[i]=D_X_Y[i+9];
        yy[i]=D_X_Y[i+18];
    }

    z=f_add(xx,f_mult(yy,VF(a,b)));
    for(i=0;i<9;i++)
    {
        d_x_y[i]=D_X_Y[i];
        d_x_y[i+9]=yy[i];
        d_x_y[i+18]=z[i];
    }

    return d_x_y;
}

int *inv(int *a)
{
    int *unity,*d,*inverse,i;
    unity=new int [9];
    inverse=new int [9];
    d=new int [27];

    unity[0]=1; unity[1]=1; unity[2]=0; unity[3]=1; unity[4]=1; unity[5]=0; unity[6]=0; unity[7]=0; unity[8]=1;

    d=aux_inv(unity,a);

    for(i=0;i<9;i++)
    {
        inverse[i]=d[18+i];
    }

    return inverse;
}

int *convert_89(int *a)
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

int *convert_98(int *A)
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

    A=convert_89(a);

    if(!(is_it_0(A))) A=inv(A);

    a=convert_98(A);

    c[0]=1; c[1]=1; c[2]=0; c[3]=0;
    c[4]=0; c[5]=1; c[6]=1; c[7]=0;

    for(i=0;i<8;i++)
    {
        b[i]=(a[i]+a[(i+4)%8]+a[(i+5)%8]+a[(i+6)%8]+a[(i+7)%8]+c[i])%2;
    }

    return b;
}

int ***mix_columns(int ***s, int n)
{
    int i,*x,*x1;
    int *t0,*t1,*t2,*t3;
    x=new int [9];
    x1=new int [9];

    t0=new int [9]; t1=new int [9]; t2=new int [9]; t3=new int [9];

    t0=convert_89(s[0][n]); t1=convert_89(s[1][n]);
    t2=convert_89(s[2][n]); t3=convert_89(s[3][n]);

    x[0]=0;     x1[0]=1;
    x[1]=1;     x1[1]=1;
    x[2]=0;     x1[2]=0;
    x[3]=0;     x1[3]=0;
    x[4]=0;     x1[4]=0;
    x[5]=0;     x1[5]=0;
    x[6]=0;     x1[6]=0;
    x[7]=0;     x1[7]=0;
    x[8]=0;     x1[8]=0;


    s[0][n]=convert_98(f_add(t3,f_add(t2,f_add(f_mult(x1,t1),f_mult(x,t0)))));  s[1][n]=convert_98(f_add(t0,f_add(t3,f_add(f_mult(x,t1),f_mult(x1,t2)))));
    s[2][n]=convert_98(f_add(t1,f_add(t0,f_add(f_mult(x1,t3),f_mult(x,t2)))));  s[3][n]=convert_98(f_add(t2,f_add(t1,f_add(f_mult(x1,t0),f_mult(x,t3)))));

    return s;
}

int *equate(int *a)
{
    return a;
}

int ***shift_rows(int ***s)
{
    int *x;
    x=new int [8];

    x=equate(s[1][0]);
    s[1][0]=equate(s[1][1]); s[1][1]=equate(s[1][2]); s[1][2]=equate(s[1][3]);
    s[1][3]=equate(x);

    x=equate(s[2][0]);
    s[2][0]=equate(s[2][2]);
    s[2][2]=equate(x);
    x=equate(s[2][1]);
    s[2][1]=equate(s[2][3]);
    s[2][3]=equate(x);

    x=equate(s[3][3]);
    s[3][3]=equate(s[3][2]); s[3][2]=equate(s[3][1]); s[3][1]=equate(s[3][0]);
    s[3][0]=equate(x);

    return s;
}

int *w_add(int *a, int *b)    //adds 2 32-bit strings.
{
    int *c,i;
    c=new int [32];

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
        if (i%4) temp=w_add(Rcon[i/4],sub_word(rot_word(temp)));
        w[i]=w_add(w[i-4],temp);
    }

    return w;
}

int ***add_subkey(int ***state, int *x, int j)
{
    int *s_word,i;
    s_word=new int [32];

    for(i=0;i<4;i++)
        copy_s2L(state[j][i],8*i,s_word);

    s_word=w_add(s_word,x);

    for(i=0;i<4;i++)
        state[j][i]=copy_L2s(s_word,8*i);

    return state;
}

int ***Add_roundkey(int ***state, int **w, int j)
{
    int i;

    for(i=0;i<4;i++)
        state=add_subkey(state,w[j+i],i);

    return state;
}

int ***subbytes_on_state(int ***state)
{
    int i,j;

    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
            state[i][j]=sub_bytes(state[i][j]);
    }

    return state;
}

int ***mixcolumns_on_state(int ***state)
{
    int i;

    for(i=0;i<4;i++)
        state=mix_columns(state,i);

    return state;
}

int ***AES_encrypt(int ***state, int **key)
{
    int **w,i;
    w=new int *[44];

    for(i=0;i<44;i++)
        w[i]=new int [8];

    w=key_expansion(key);

    state=Add_roundkey(state,w,0);

    for(i=1;i<10;i++)
    {
        state=subbytes_on_state(state);
        state=shift_rows(state);
        state=mixcolumns_on_state(state);
        state=Add_roundkey(state,w,i);
    }

    state=subbytes_on_state(state);
    state=shift_rows(state);
    state=Add_roundkey(state,w,10);

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

    cout<<"\nenter the plaintext\n\n";

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

    cout<<"\nenter the key \n\n";
    for(i=0;i<16;i++)
    {
        for(j=0;j<8;j++)
            cin>>key[i][j];
    }

    cout<<"\nplaintext = \n\n";
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

    state=AES_encrypt(state,key);

    cout<<"\ciphertext = \n\n";
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
