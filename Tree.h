//
//  Header.h
//  Akinator
//
//  Created by Илья  on 28/11/2018.
//  Copyright © 2018 Илья . All rights reserved.
//

#ifndef Tree_h
#define Tree_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

struct node
{
    char* val;
    struct node* previous;
    struct node* left;
    struct node* right;
};

struct node* nodecreate();
void treepush (struct node* root, char* value, int sentsize, char marker);
void read_file (struct node* root, FILE* in, FILE* in_copied);
void reader (struct node* root, FILE* in, char* vvod, int* counter, struct node* head);
int str_size (char* buf, int position);
char* get_str (char* buf, int* position, int size);
void write_file (struct node* root, FILE* in, int tabs);
void print_tabs (FILE* in, int tabs);
void make_dot (FILE* tree, struct node* root);
void tree_distract (struct node* root);

struct node* nodecreate()
{
    struct node* root = (struct node*)calloc(1, sizeof(struct node));
    root -> val = NULL;
    root -> left = NULL;
    root -> right = NULL;
    root -> previous = NULL;
    return root;
}

void treepush (struct node* root, char* value, int sentsize, char marker)
{
    if (root -> val == NULL)
    {
        root -> val = (char*)calloc(sentsize + 1, sizeof(char));
        root -> val = value;
    }
    else if (marker == 'l')
    {
        if (root -> left == NULL)
        {
            struct node* node = nodecreate();
            node -> val = (char*)calloc(sentsize, sizeof(char));
            node -> val = value;
            root -> left = node;
            node -> previous = root;
        }
        else
        {
            treepush(root -> left, value, sentsize, marker);
        }
    }
    else
    {
        if (root -> right == NULL)
        {
            struct node* node = nodecreate();
            node -> val = (char*)calloc(sentsize, sizeof(char));
            node -> val = value;
            root -> right = node;
            node -> previous = root;
        }
        else
        {
            treepush(root -> right, value, sentsize, marker);
        }
    }
}

void read_file (struct node* root, FILE* in, FILE* in_copied)
{
    fseek(in, 0, SEEK_END);
    long size = ftell(in);
    fseek(in, 0, SEEK_SET);
    char* vvod = (char*)calloc(size + 2, sizeof(char));
    fread(vvod, sizeof(char), size, in);
    
    fwrite(vvod, sizeof(char), size, in_copied);
    
    int counter1 = 0;
    reader(root, in, vvod, &counter1, root);
}

void reader (struct node* root, FILE* in, char* vvod, int* counter, struct node* head)
{
    for (; vvod[*counter] != '\0'; *counter += 1)
    {
        if (vvod[*counter] == '{')
        {
            *counter += 1;
            
            int sent_size = str_size(vvod, *counter);
            char* sentence = (char*)calloc(sent_size, sizeof(char));
            sentence = get_str(vvod, counter, sent_size);
            
            treepush(root, sentence, sent_size, 'l');
            if (vvod[*counter] == '{' && root->left != NULL) reader(root -> left, in, vvod, counter, head);
            else if (vvod[*counter] == '{' && root -> previous != NULL)
            {
                reader(root -> previous, in, vvod, counter, head);
            }
            else if (vvod[*counter] == '{')
            {
                reader(head, in, vvod, counter, head);
            }
        }
        
        if (vvod[*counter] == '}')
        {
            for (; vvod[*counter] != '{' && vvod[*counter] != '\0'; *counter += 1)
            {
                if (root -> previous != NULL && vvod[*counter + 1] == '}') root = root -> previous;
            }
            if (vvod[*counter] == '\0') continue;
            *counter += 1;
            
            int sent_size = str_size(vvod, *counter);
            char* sentence = (char*)calloc(sent_size, sizeof(char));
            sentence = get_str(vvod, counter, sent_size);
            
            treepush(root, sentence, sent_size, 'r');
            if (vvod[*counter] == '{' && root -> right != NULL) reader(root -> right, in, vvod, counter, head);
            else if (vvod[*counter] == '}' && root -> previous != NULL)
            {
                reader(root, in, vvod, counter, head);
            }
            else if (vvod[*counter] == '}')
            {
                reader(head, in, vvod, counter, head);
            }
        }
    }
}


int str_size (char* buf, int position)
{
    int sent_size = 0;
    for (;buf[position] != '{' && buf[position] != '}' && buf[position] != '\0'; position++)
    {
        sent_size++ ;
    }
    
    return sent_size;
}

char* get_str (char* buf, int* position, int size)
{
    char* sentence = (char*)calloc(size, sizeof(char));
    int counter = 0;
    
    for (;buf[*position] != '{' && buf[*position] != '}' && buf[*position ] != '\0'; *position += 1)
    {
        if (buf[*position] == '\t' || buf[*position] == '\n') continue;
        
        sentence[counter] = buf[*position];
        counter++ ;
    }
    
    return sentence;
}

void write_file (struct node* root, FILE* in, int tabs)
{
    if (root -> val != NULL)
    {
        fprintf(in, "{\n");
        print_tabs(in, tabs);
        fprintf(in, "%s\n", root -> val);
        
        if (root -> left != NULL)
        {
            print_tabs(in, tabs);
            write_file(root -> left, in, tabs + 1);
            print_tabs(in, tabs);
            fprintf(in, "}\n");
        }
        
        if (root -> right != NULL)
        {
            print_tabs(in, tabs);
            write_file(root -> right, in, tabs + 1);
            print_tabs(in, tabs);
            fprintf(in, "}\n");
        }
    }
}

void print_tabs (FILE* in, int tabs)
{
    int counter = 0;
    for (;counter < tabs; counter++)
    {
        fprintf(in, "\t");
    }
}

void make_dot (FILE* tree, struct node* root)
{
    if (root -> left != NULL && root -> right != NULL)
    {
        fprintf(tree, "%s -> %s[color=red];\n%s -> %s[color=green];\n", root -> val, root -> right -> val, root -> val, root -> left -> val);
        make_dot(tree, root -> left);
        make_dot(tree, root -> right);
    }
}

void tree_distract (struct node* root)
{
    if (root -> left != NULL)
    {
        tree_distract(root -> left);
        root -> left = NULL;
    }
    
    if (root -> right != NULL)
    {
        tree_distract(root-> right);
        root -> right = NULL;
    }
    
    if ((root -> right = NULL) && (root -> left = NULL))
    {
        free(root -> val);
        free(root);
    }
}

#endif
