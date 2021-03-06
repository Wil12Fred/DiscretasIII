class RSA{
  private:
    ZZ p,q,d,phi;//claves privadas del RSA
    ZZ phi2;//phiEuler(phi)-1;inversa Fermat-Euler
    ZZ dp,dq,iq,ip;//Teorema del resto chino
  public:
    ZZ n,e;//claves publicas del RSA
    string diccionario;//diccionario publico

    ZZ inversa(ZZ &a){//Fermat-Euler
      return ExpMod(a,phi2,phi);//inv(n,m)=n^(phi(m)-1,m)
    }

    ZZ tcr(ZZ &m){
      dp=ExpMod(m,d%(p-1),p);
      dq=ExpMod(m,d%(q-1),q);
      return (dp*q*iq+dq*p*ip)%n;
    }

    RSA(string diccionario){//Constructor
      this->diccionario=diccionario;
      p=GPrime(tam/2);//Gordon(tam*3/8)
      q=GPrime(tam/2);//Gordon(tam*3/8)
      cout << "p: " << endl;
      cout << p << endl;
      cout << "q: " << endl;
      cout << q << endl;
      iq=inv(q,p);
      ip=inv(p,q);
      n=p*q;
      phi=(p-1)*(q-1);
      phi2=phiEuler(phi)-1;
      e=BlumBlumShub(phi>>tam*3/8);//e=3;
      if ((e&1)==0) e++;
      while(gcd(e,phi)!=1){
        e+=2;//e=BlumBlumShub(phi>>tam*3/8);
      }
      //d=inv(e,phi);
      d=inversa(e);
      cout << n << endl;
      cout << e << endl;
      cout << "d: " << endl;
      cout << d << endl;
    }

    void recuperar(){//Recuperacion
      ifstream leer ("Guardar.txt",ios::in);
      leer >> diccionario;
      leer >> p;
      leer >> q;
      leer >> e;
      cout << p << endl;
      cout << q << endl;
      iq=inv(q,p);
      ip=inv(p,q);
      n=p*q;
      phi=(p-1)*(q-1);//phi2=phiEuler(phi)-1;
      cout << "n: "<< n  << endl;
      cout << "e: " << e << endl;
    }

    string encriptar(string mns,ZZ n_b,ZZ e_b){
      ofstream darMensaje("Mensaje.txt",ios::out);
      ZZ m,c,ans,aux,fd,pos;
      string enc,auxiliar;

      for (int i=0;i<mns.size();i++){
        m=conv<ZZ> (diccionario.find(mns[i]));
        auxiliar+=(ToString(m,2));
      }
      int tam=(ele(n_b)-1);
      int resto=auxiliar.size()%tam;
      if(resto!=0){
        int falta=tam-resto;
        for (int i=0;i<falta;i++){
          auxiliar.push_back('0');
        }
      }
      for (int i=0;i<auxiliar.size();i+=tam){
        string auxiliar1(auxiliar,i,tam);
        ZZ m1(INIT_VAL, auxiliar1.c_str());
        c=ExpMod(m1,e_b,n_b);
        enc+=(ToString(c,ele(n_b)));
      }
      darMensaje << enc << endl;
      darMensaje.close();
      return enc;
    }

    string desencriptar(string mns){
      string ans,aux_1;
      ZZ m;
      for (int i=0;i<mns.size();i+=ele(n)){
        string aux(mns,i,ele(n));
        ZZ c(INIT_VAL, aux.c_str());
        m=this->tcr(c);
        aux_1+=(ToString(m,ele(n)-1));
      }
      for (int i=0;i<aux_1.size();i+=2){
        string aux(aux_1,i,2);
        ZZ c(INIT_VAL, aux.c_str());
        ans.push_back(diccionario[conv<int>(c)]);
      }
      return ans;
    }

    void dar_datos(){
      ofstream CopDat("Datos.txt",ios::out);
      CopDat << "n: " << n << endl;
      CopDat << "e: " << e << endl;
      CopDat.close();
      ofstream copiar("Guardar.txt",ios::out);
      copiar << diccionario << endl;
      copiar << p << endl;
      copiar << q << endl;
      copiar << e << endl;
      copiar.close();
      return;
    }
    string darFD(char mns2,ZZ n_b,ZZ e_b){
      ZZ fd=to_ZZ(diccionario.find(mns2));
      fd=tcr(fd);
      string rubrica=ToString(fd);
      string r;
      while ((rubrica.size()+r.size())%(ele(n_b)-1)){
        r.push_back(0);
      }
      r+=rubrica;
      string firma,aux;
      for (int i=0;i<r.size();i+=ele(n_b)-1){//aqui
        string bloque(r,i,ele(n_b)-1);//aqui
        ZZ c(INIT_VAL,bloque.c_str());
        c=ExpMod(c,e_b,n_b);
        firma+=ToString(c,ele(n_b));
      }
      return firma;
    }
    char verFD(string fd,ZZ n_b, ZZ e_b){
      string firma,aux;
      for (int i=0;i<fd.size();i+=ele(n)){
        string bloque(fd,i,ele(n));
        ZZ c(INIT_VAL,bloque.c_str());
        c=tcr(c);
        firma+=ToString(c,ele(n)-1);//aqui
      }
      ZZ c(INIT_VAL,firma.c_str());;
      c=ExpMod(c,e_b,n_b)%diccionario.size();
      return diccionario[conv<int>(c)];
    }
};
