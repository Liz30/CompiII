#include <cstdio>
#include "ast.h"

// Si id esta en vars_type ya existe
map<string, Type_v> vars_type; //id, tipo
map<string, int> size_arrays; // el tipo esta en vars_type, este es solo para saber el size
map<string, int> vars_value; //contiene el valor de cada id, el resultado de evaluate()
map<string, int*> arrays_value; // id, int[]. Valores de los arreglos

bool v_main = false;
map<string, Type_v> methods_type;
map<string, string> methods_var;
map<string, Type_v> vars_type_temp;
map<string, int> size_arrays_temp; // el tipo esta en vars_type, este es solo para saber el size
map<string, int> vars_value_temp; //contiene el valor de cada id, el resultado de evaluate()
map<string, int*> arrays_value_temp; // id, int[]. Valores de los arreglos

map<string, int> methods_on;  // 1 activo, 0 inactivo (lugar libre)


void BlockStatement::execute()
{
    printf("BlockStatement\n");
    /*list<Statement *>::iterator it = stList.begin();

    while (it != stList.end()) {
        Statement *st = *it;

        st->execute();
        it++;
        printf("%s\n", st->getKind());
    }*/
}

void PrintStatement::execute()
{
    printf("Print Beginning\n");
    ExprList::iterator it = expr->begin();
    int result;

    while (it != expr->end()) {
      Expr *ex = *it;

      result=ex->evaluate();
      printf("%d\n", result);
      it++;
    }
    printf("\n");
}

void AssignStatement::execute()
{
    printf("\nAssignStatement\n");

    if (type_v != _NULL){   // aun no se ha declarado
      ExprList *ne = new ExprList;
      ne->push_back(new IdExpr(type_v, id));
      DeclarationStatement *n = new DeclarationStatement(type_v, ne);
      n->execute();
    }
    int result = expr->evaluate();
    vars_value[id] = result;

    cout<<" ID : "<<id<<"  Tipo: "<<vars_type[id]<<"  Result: "<<vars_value[id]<<"\n";
}

void AssignStatementArray::execute()
{
  printf("\n\nAssignStatementArray\n");
  if (type_v != _NULL){   // caso: int y[6] = 36; Incorrecto
    ErrorExpr *e = new ErrorExpr(SEMANTIC, " \n");
    e->show();
  }
  else{
    int r = expr->evaluate();
    int p = pos->evaluate();
    // Evaluar tipo??
    arrays_value[id][p] = r;
  }
  // testing
  for (int j=0; j<size_arrays[id]; j++)
      cout<<" value["<<id<<"]["<<j<<"]: "<<arrays_value[id][j]<<"\n";
}

void IfStatement::execute()
{
    printf("IfStatement\n");
    int result = cond->evaluate();

    if (result) {
        trueBlock->execute();
    } else if (falseBlock != 0) {
        falseBlock->execute();
    }
}

void WhileStatement::execute()
{
  printf("WhileStatement\n");
  while (cond->evaluate()) {
    block->execute();
  }
}

void ForStatement::execute()
{
  printf("ForStatement\n");
  /*for ( init->execute(); cond->evaluate(); count->execute()) {
    block->execute();
  }*/
}

void DeclarationStatement::execute()
{
  printf("DeclarationStatement\n");

  ExprList::iterator it = ilist->begin();
  while (it != ilist->end()){
    Expr *e = *it;
    if (e->getKind()==ID_EXPRESSION){
        IdExpr *ie = (IdExpr*)e;
        string i = ie->id;
        vars_type[i] = type_v;
        vars_value[i] = 0;
        cout<<" ID : "<<i<<"  Tipo: "<<vars_type[i]<<"\n";
    } else {
        if (e->getKind()==ARRAY_EXPRESSION){
            ArrayExpr *ae = (ArrayExpr*)e;
            string i = ae->id;
            if (ae->size > 0){
                size_arrays[i] = ae->size;
                vars_type[i] = type_v;
                arrays_value[i] = new int[ae->size];

                cout<<" ID: "<<i<<"  Tipo: "<<vars_type[i]<<"  Size: "<<size_arrays[i]<<"\n";
                for (int y=0; y<size_arrays[i]; y++)
                    arrays_value[i][y] = 0;
                for (int j=0; j<size_arrays[i]; j++)
                    cout<<" value["<<i<<"]["<<j<<"]: "<<arrays_value[i][j]<<"\n";
            }
            else{
                ErrorExpr *_error = new ErrorExpr(SINTAX, " Expected size greater than zero");
              _error->show();
            }
        }
        else{
                ErrorExpr *_error = new ErrorExpr(SEMANTIC, " Expression not recognized");
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

  /*Type_v type;
  string id;
  ExprList *param;
  Statement *block;*/

  if ( (id == "MAIN") && v_main ){ // Es void
    // definir main
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
