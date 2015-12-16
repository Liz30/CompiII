#include <cstdio>
#include "ast.h"

//enum Type_v { INT, BOOL, VOID, _NULL };

map<string, Type_v> vars_type; //id, tipo
map<string, int> size_arrays; // el tipo esta en vars_type, este es solo para saber el size
map<string, int> vars_value; //contiene el valor de cada id, el resultado de evaluate()



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
    printf("AssignStatement\n");
    //int result = expr->evaluate();
    //vars_type[id] = result;
}

void AssignStatementArray::execute()
{
  printf("AssignStatementArray\n");
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
        cout<<" ID Expression. ID: "<<i<<"  Tipo: "<<vars_type[i]<<"\n";
    } else {
        if (e->getKind()==ARRAY_EXPRESSION){
            ArrayExpr *ae = (ArrayExpr*)e;
            string i = ae->id;
            size_arrays[i] = ae->size;
            vars_type[i] = type_v;
            cout<<" Array Expression. ID: "<<i<<"  Tipo: "<<vars_type[i]<<"  Size: "<<size_arrays[i]<<"\n";
        }
        else{
                ErrorExpr *_error = new ErrorExpr(SEMANTIC, " Expression not recognized");
                _error->show();
        }
    }
    it++;
  }
  PrintStatement *p = new PrintStatement(ilist);
  p->execute();
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
  printf("MethodStatement\n");
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
