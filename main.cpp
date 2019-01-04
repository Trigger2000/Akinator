//
//  main.cpp
//  Akinator
//
//  Created by Илья  on 28/11/2018.
//  Copyright © 2018 Илья . All rights reserved.
//

#include "Tree.h"
#include <iostream>
#define destination1 "/Users/ilya1/Desktop/code/Akinator/Akinator/in.txt"
#define destination2 "/Users/ilya1/Desktop/code/Akinator/Akinator/in_copied.txt"
#define destination3 "/Users/ilya1/Desktop/code/Akinator/Akinator/tree.dot"

void game (struct node* root);
void addelement (struct node** node);
void repeat_function (struct node* root);

int main()
{
    FILE* in = fopen(destination1, "r");
    FILE* in_copied = fopen(destination2, "w+");
    FILE* tree = fopen(destination3, "w+");
    assert(in != NULL);
    assert(in_copied != NULL);
    assert(tree != NULL);
    
    struct node* root = nodecreate();
    read_file(root, in, in_copied);
    fclose(in_copied);
    fclose(in);
    
    in = fopen(destination1, "w+");
    assert(in != NULL);
    
    printf("Type <y> for YES and other symbols for NO\n");
    repeat_function(root);
    
    write_file(root, in, 1);
    fprintf(in, "}");
    
    fprintf(tree, "digraph tree\n{\n");
    make_dot(tree, root);
    fprintf(tree, "}");
    fclose(tree);
    
    fclose(in);
    tree_distract(root);
    return 0;
}

void game (struct node* root)
{
    if (root -> left != NULL && root -> right != NULL)
    {
        printf("%s?\n", root -> val);
        char vvod = 0, xlam = 0;
        vvod = getchar();
        xlam = getchar();
        if (vvod == 'y')
        {
            game(root -> left);
        }
        else
        {
            game(root -> right);
        }
    }
    else
    {
        printf("Is it %s?\n", root -> val);
        char vvod = 0, xlam = 0;
        vvod = getchar();
        xlam = getchar();
        if (vvod == 'y') printf("I knew it!\n");
        else addelement(&root);
    }
}

void repeat_function (struct node* root)
{
    game(root);
    char vvod = 0, xlam = 0;
    printf("Would you like to play again?\n");
    vvod = getchar();
    xlam = getchar();
    if (vvod == 'y') repeat_function(root);
}

void addelement (struct node** node)
{
    char* question = (char*)calloc(1000, sizeof(char));
    char* name = (char*)calloc(100, sizeof(char));
    int questionlength = 0, namelength = 0, length = 0;
    
    printf("I'm giving in... Who it was?\n");
    scanf("%s", name);
    //std::cin.getline(name, 100);
    printf("What's the difference between %s and %s?\n", name, (*node) -> val);
    scanf("%s", question);
    //std::cin.getline(question, 1000);
    
    for (; name[namelength] != '\0'; namelength++);
    for (questionlength = 0; question[questionlength] != '\0'; questionlength++);
    for (length = 0; ((*node) -> val)[length] != '\0'; length++);
    
    struct node* node1 = nodecreate();
    node1 -> val = (char*)calloc(namelength + 3, sizeof(char));
    node1 -> val = name;
    (*node) -> left = node1;
    
    struct node* node2 = nodecreate();
    node2 -> val = (char*)calloc(length + 3, sizeof(char));
    node2 -> val = (*node) -> val;
    (*node) -> right = node2;
    
    //(*node) -> val = (char*)realloc((*node) -> val, questionlength + 3);
    (*node) -> val = question;
}
