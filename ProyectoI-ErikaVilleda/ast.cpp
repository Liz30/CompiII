#include <cstdio>
#include "ast.h"

extern int line;

// Si id esta en vars_type ya existe
map<string, Type_v> vars_type; //id, tipo
map<string, int> size_arrays; // el tipo esta en vars_type, este es solo para saber el size
map<string, int> vars_value; //contiene el valor de cada id, el resultado de evaluate()
map<string, int*> arrays_value; // id, int[]. Valores de los arreglos

bool v_main = false;
map<string, Type_v> methods_type; // Tipo de cada metodo
map<string, string> methods_var; // idMethod, id. Variables de cada metodo
map<string, int> methods_value; // IDMethod, value.  Valor que retorna cada metodo.
map<string, Type_v> vars_type_temp;
//map<string, int> size_arrays_temp; // el tipo esta en vars_type, este es solo para saber el size
map<string, int> vars_value_temp; //contiene el valor de cada id, el resultado de evaluate()
//map<string, int*> arrays_value_temp; // id, int[]. Valores de los arreglos
map<string, string> vars_methods;

map<string, int> methods_on;  // 1 activo, 0 inactivo (lugar libre)
string currentMethod;

bool existVar(string id) {// Globales
    map<string, Type_v>::iterator it = vars_type.find(id);
    if (it != vars_type.end())
        return true;
    return false;
}

bool existVarTemp(string id) {// Temporales
    map<string, string>::iterator it = vars_methods.find(id);
    if (it != vars_methods.end())
        return true;
    return false;
}

bool existMeth(string id){
    map<string, Type_v>::iterator it = methods_type.find(id);
    if ( it != methods_type.end())
        return true;
    return false;
}

bool isArray(string id){
  map<string, int>::iterator it = size_arrays.find(id);
  if ( it != size_arrays.end())
      return true;
  return false;
}

int IdExpr::evaluate()
{
    if (existVar(id)) // Es Global
      return vars_value[id];
    return vars_value_temp[id];
}

void BlockStatement::execute()
{
    printf("\nBlockStatement\n");
    if (declaration > 0){
        cout<<"      Declaration Block\n";
        StatementList::iterator it = declaration->begin(); // Declaration block
        while (it != declaration->end()) {
            Statement *st = *it;
            st->execute();
            it++;
        }
    }
    else{
        WarningExpr *w = new WarningExpr("There are not DeclarationBlock", line);
        w->show();
    }

    if (stList > 0){
        cout<<"      Code Block\n";
        StatementList::iterator it = stList->begin();  // Code block
        while (it!=stList->end()) {
          Statement *st = *it;
          st->execute();
          it++;
        }
    }
    else{
        WarningExpr *w = new WarningExpr("There are not CodeBlock", line);
        w->show();
    }

}

void PrintStatement::execute()
{
    printf("Print Beginning\n");
    ExprList::iterator it = expr->begin();
    int result;

    while (it != expr->end()) {
      Expr *ex = *it;
      if (ex->getKind()==ID_EXPRESSION){
          IdExpr *nuevo = (IdExpr*)ex;
          if (isArray(nuevo->id)){
              int s = size_arrays[nuevo->id];
              for(int o=0; o<s; o++ ){
                  cout<<"["<<nuevo->id<<"]["<<o<<"]: "<<arrays_value[nuevo->id][o]<<"\n";
              }
          }
          else{
              if (vars_methods[nuevo->id]==currentMethod){
                  result = ex->evaluate();
                  cout<<"Result: "<<result<<"\n";
              }
              else{
                  ErrorExpr *e = new ErrorExpr(SEMANTIC, "ID: \'"+nuevo->id+"\' is not found", line);
                  e->show();
              }
          }
      }
      else{
          result = ex->evaluate();
          cout<<"Result: "<<result<<"\n";
      }
      it++;
    }
}

void AssignStatement::execute()
{
    printf("\nAssignStatement\n");

    if (type_v != _NULL){   // aun no se ha declarado int a = 9;
      ExprList *ne = new ExprList;
      ne->push_back(new IdExpr(type_v, id));
      DeclarationStatement *n = new DeclarationStatement(type_v, ne);
      n->execute();
    }
    if ( existVar(id) ) { // La encontro, comparar si es arreglo. Global
        int result = expr->evaluate();
        vars_value[id] = result;
        cout<<" ID : "<<id<<"  Tipo: "<<vars_type[id]<<"  Result: "<<vars_value[id]<<"\n";
    }
    else{
        if ( existVarTemp(id) ) {//Es temporal
          if (vars_methods[id] == currentMethod){
              int result = expr->evaluate();
              vars_value_temp[id] = result;
              cout<<" ID : "<<id<<"  Tipo: "<<vars_type_temp[id]<<"  Result: "<<vars_value_temp[id]<<"\n";
          }
          else{
              ErrorExpr *e=new ErrorExpr(SEMANTIC, "CurrentMethod: \'"+currentMethod+"\' doesn't have ID: "+id, line);
              e->show();
          }
        }
        else{
            ErrorExpr *e = new ErrorExpr(SEMANTIC, " ID: \'"+id+"\' is not found\n", line);
            e->show();
        }
    }
//    cout<<currentMethod<<"\n";
}

void AssignStatementArray::execute()
{
  printf("\n\nAssignStatementArray\n");
  if (type_v != _NULL){   // caso: int y[6] = 36; Incorrecto
    ErrorExpr *e = new ErrorExpr(SEMANTIC, " \n", line);
    e->show();
  }
  else{
    if ( !existVar(id) ){ // Tiene que ser global
          ErrorExpr *e = new ErrorExpr(SEMANTIC, " ID: "+id+" is not found \n", line);
          e->show();
    }
    else{
        int r = expr->evaluate();
        int p = pos->evaluate();
        // Evaluar tipo??
        if (p >= size_arrays[id]){
           ErrorExpr *e = new ErrorExpr(SEMANTIC, "Position is greater than size of \'"+id+"\'", line);
           e->show();
        }
        else{
            arrays_value[id][p] = r;
            for (int j=0; j<size_arrays[id]; j++)
                cout<<" value["<<id<<"]["<<j<<"]: "<<arrays_value[id][j]<<"\n";
        }
    }
  }
}

void IfStatement::execute()
{
    printf("IfStatement\n");
    /*int result = cond->evaluate();

    if (result) {
        trueBlock->execute();
    } else if (falseBlock != 0) {
        falseBlock->execute();
    }*/
}

void WhileStatement::execute()
{
  printf("WhileStatement\n");
  //while (cond->evaluate()) {
    //block->execute();
  //}
}

void ForStatement::execute()
{
  printf("ForStatement\n");
  /*for ( init->execute(); cond->evaluate(); count->execute()) {
    block->execute();
  }*/
}

void DeclarationStatement::execute() // Variables Globales
{
  printf("DeclarationStatement\n");

  ExprList::iterator it = ilist->begin();
  while (it != ilist->end()){
    Expr *e = *it;
    if (e->getKind()==ID_EXPRESSION){
        IdExpr *ie = (IdExpr*)e;
        string i = ie->id;
        if ( !existVar(i) ){
            vars_type[i] = type_v;
            vars_value[i] = 0;
            methods_var["class"] = i;
            vars_methods[i] = "class";
            cout<<" ID : "<<i<<"  Tipo: "<<vars_type[i]<<"\n";
        }
        else{
            ErrorExpr *e = new ErrorExpr(SEMANTIC, " Redeclaration of ID: "+i, line);
            e->show();
        }
    } else {
        if (e->getKind()==ARRAY_EXPRESSION){
            ArrayExpr *ae = (ArrayExpr*)e;
            string i = ae->id;
            if ( ! existVar (i) ){
                if (ae->size > 0){
                    size_arrays[i] = ae->size;
                    vars_type[i] = type_v;
                    arrays_value[i] = new int[ae->size]; // arreglo[size]
                    methods_var["class"] = i;
                    vars_methods[i] = "class";

                    cout<<" ID: "<<i<<"  Tipo: "<<vars_type[i]<<"  Size: "<<size_arrays[i]<<"\n";
                    for (int y=0; y<size_arrays[i]; y++)
                        arrays_value[i][y] = 0;
                    for (int j=0; j<size_arrays[i]; j++)
                        cout<<" value["<<i<<"]["<<j<<"]: "<<arrays_value[i][j]<<"\n";
                }
                else{
                    ErrorExpr *_error = new ErrorExpr(SINTAX, " Expected size greater than zero", line);
                  _error->show();
                }
            }
            else{
                ErrorExpr *e = new ErrorExpr(SEMANTIC, " Redeclaration of ID: "+i, line);
                e->show();
            }
        }
        else{
                ErrorExpr *_error = new ErrorExpr(SEMANTIC, " Expression not recognized", line);
                _error->show();
        }
    }
    it++;
  }
}

void ProgramStatement::execute()
{
  printf("\nProgramStatement\n\n");

  StatementList::iterator it = header->begin();
  while (it != header->end()) {
    Statement *st = *it;
    st->execute();
    it++;
  }

  StatementList::iterator itm = methods->begin();
  while (itm != methods->end()) {
    Statement *st = *itm;

    st->execute();
    itm++;
  }
}

void MethodStatement::execute()
{
  printf("\n\nMethodStatement\n");
  if ( existMeth(id) ){
      ErrorExpr *e = new ErrorExpr(SEMANTIC, "Redefinition of Method Id: "+id, line);
      e->show();
  }
  else{
      if (type == VOID){
          if (id == "MAIN"){
              if (v_main){
                ErrorExpr *e = new ErrorExpr(SEMANTIC, "Redefinition of VOID MAIN", line);
                e->show();
              }
              else{
                // definir main
                  if (param == 0){
                      v_main = true;
                      methods_type[id] = type;
                      currentMethod = id;
                      if ( block->declaration == 0){
                        WarningExpr *w = new WarningExpr("There are not DeclarationBlock", line);
                        w->show();
                      }
                      else{
                          StatementList::iterator itt = block->declaration->begin();
                          while ( itt != block->declaration->end()){
                              DeclarationStatement *nst = (DeclarationStatement*)*itt;
                              inMethod(nst->type_v, nst->ilist);
                              itt++;
                          }
                      }
                      if ( block->stList == 0){
                        WarningExpr *w = new WarningExpr("There are not Code Block", line);
                        w->show();
                      }
                      else{
                          StatementList::iterator itt = block->stList->begin();
                          while ( itt != block->stList->end()){
                              Statement *nst = *itt;
                              nst->execute();
                              itt++;
                          }
                      }
                  }
                  else{
                      ErrorExpr *e=new ErrorExpr(SEMANTIC, "Unexpected Parameters in Void Main\n", line);
                      e->show();
                  }
              }
          }
          else{
            // definir void id
              methods_type[id] = type;
              currentMethod = id;
              if (param > 0)
                  SetParamenters(param);
              if ( block->declaration == 0){
                WarningExpr *w = new WarningExpr("There are not DeclarationBlock", line);
                w->show();
              }
              else{
                  StatementList::iterator itt = block->declaration->begin();
                  while ( itt != block->declaration->end()){
                      DeclarationStatement *nst = (DeclarationStatement*)*itt;
                      inMethod(nst->type_v, nst->ilist);
                      itt++;
                  }
              }
              if ( block->stList == 0){
                WarningExpr *w = new WarningExpr("There are not Code Block", line);
                w->show();
              }
              else{
                  StatementList::iterator itt = block->stList->begin();
                  while ( itt != block->stList->end()){
                      Statement *nst = *itt;
                      nst->execute();
                      itt++;
                  }
              }
           }
         }
      else{ // function
          methods_type[id] = type;
          currentMethod = id;
          if (param > 0)
              SetParamenters(param);
          if (block->declaration == 0){
            WarningExpr *w = new WarningExpr("There are not DeclarationBlock", line);
            w->show();
          }
          else{
              StatementList::iterator itt = block->declaration->begin();
              while ( itt != block->declaration->end()){
                  DeclarationStatement *nst = (DeclarationStatement*)*itt;
                  inMethod(nst->type_v, nst->ilist);
                  itt++;
              }
          }
          if ( block->stList == 0){
            WarningExpr *w = new WarningExpr("There are not Code Block", line);
            w->show();
          }
          else{
              StatementList::iterator itt = block->stList->begin();
              while ( itt != block->stList->end()){
                  Statement *nst = *itt;
                  nst->execute();
                  itt++;
              }
          }
        }
  }
}

void MethodStatement::SetParamenters(ExprList *eL)
{
  cout<<" SetParamenters\n";

  cout<<" Parameters in Method: "<<id<<"\n";
  ExprList::iterator it = eL->begin();
  while (it != eL->end()){
    Expr *e = *it;
    if (e->getKind()==ID_EXPRESSION){
        IdExpr *ie = (IdExpr*)e;
        string i = ie->id;
        if ( !existVar(i) ){ // No es global
            vars_type_temp[i] = ie->t;
            vars_value_temp[i] = 0;
            methods_var[id] = i;
            vars_methods[i] = id;
            cout<<" ID : "<<i<<"  Tipo: "<<vars_type_temp[i]<<"\n";
        }
        else{
            ErrorExpr *e = new ErrorExpr(SEMANTIC, " ID: "+i+" is Global Var", line);
            e->show();
        }
    } else {
        ErrorExpr *e = new ErrorExpr(SEMANTIC, " Expression is not recognized.", line);
        e->show();
    }
    it++;
  }
}

bool MethodStatement::existVarTemp(string idVar) {
    map<string, Type_v>::iterator it = vars_type_temp.find(idVar);
    map<string, string>::iterator itt = vars_methods.find(idVar);
    if  (it != vars_type_temp.end())// && ( itt != vars_methods.end() ) )
        return true;
    return false;
}

void MethodStatement::inMethod(Type_v type, ExprList *eL)
{
    cout<<" inMethod: "<<id<<"\n";
    if ( eL == 0 ){
      WarningExpr *w = new WarningExpr("There are not DeclarationBlock", line);
      w->show();
    }
    else{
      ExprList::iterator it = eL->begin();
      while( it != eL->end()){
          Expr *e = *it;
          if (e->getKind()==ID_EXPRESSION){
              IdExpr *ie = (IdExpr*)e;
              string i = ie->id;
              if ( !existVar(i) && !existVarTemp(i) ) {
                      vars_type_temp[i] = type;
                      vars_value_temp[i] = 0;
                      methods_var[id] = i;
                      vars_methods[i] = id;
                      cout<<"     ID : "<<i<<"  Tipo: "<<vars_type_temp[i]<<"\n";
              }
              else{
                  if ( existVarTemp(i) && (vars_methods[i] != currentMethod)){
                      vars_type_temp[i] = type;
                      vars_value_temp[i] = 0;
                      methods_var[id] = i;
                      vars_methods[i] = id;
                      cout<<"     ID : "<<i<<"  Tipo: "<<vars_type_temp[i]<<"\n";
                  }
                  else{
                      ErrorExpr *e = new ErrorExpr(SEMANTIC, " Redeclaration of ID: "+i, line);
                      e->show();
                  }
              }
          }
          else{
                  ErrorExpr *_error = new ErrorExpr(SEMANTIC, " Expression not recognized", line);
                  _error->show();
          }
          it++;
      }
    }
}

void MethodCallStatement::execute()
{
  printf("MethodCallStatement\n");
}

void ReadStatement::execute()
{
  printf("ReadStatement\n");
}

void ReturnStatement::execute()
{
  printf("ReturnStatement\n");
}

void BreakStatement::execute()
{
  printf("BreakStatement\n");
}

void ContinueStatement::execute()
{
  printf("ContinueStatement\n");
}
