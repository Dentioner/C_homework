#include<stdio.h>
#include<stdlib.h>
#pragma warning(disable:4996)

#define MAXLENGTH 50
#define ADDLENGTH 10
#define link 0
#define thread 1

/*
读取两个数组，然后先读先序数组，找到根节点
然后在中序数组里面找到根节点
它的左边就是左子树，右边就是右子树
 */

typedef int elemtype;

typedef struct BiTNode
{
	elemtype data;
	struct BiTNode *lchild, *rchild, *parent;
	//int ltag, rtag;
}node, *tree;

int preorder_array[MAXLENGTH];
int inorder_array[MAXLENGTH];
int pre_len;
int in_len;



void create_array()
{
    int ch;
    int number;
    int index1, index2;
    index1 = index2 = 0;
    scanf("%d", &number);
    ch = getchar();
    while (ch != EOF && ch != '\n' && ch != ';')
    {
        preorder_array[index1++] = number;
        scanf("%d", &number);
        ch = getchar();
    }
    preorder_array[index1] = number;
    preorder_array[index1+1] = '\0';
    pre_len = index1 + 1;
    
    scanf("%d", &number);
    ch = getchar();
    while (ch != EOF && ch != '\n' && ch != ';')
    {
        inorder_array[index2++] = number;
        scanf("%d", &number);
        ch = getchar();
    }
    inorder_array[index2] = number;
    inorder_array[index2 +1] = '\0';
    in_len = index2 +1;
    if (pre_len != in_len)
        exit(1);
    

    return;
}

/*
可以考虑使用递归方式
参数为根节点的位置，以及需要处理的数组的范围
 */
void create_subtree(tree *parent, tree *sub_root, int start_index, int end_index)
{
	int index1, index2;
	if (start_index != end_index)
	{
		(*sub_root) = (tree)malloc(sizeof(node));
		(*sub_root)->data = 0;
		(*sub_root)->lchild = (*sub_root)->rchild = (*sub_root)->parent = NULL;

		for (index1 = pre_len - 1; preorder_array[index1] >= 0; index1--)
			;
		index1++;
		(*sub_root)->data = preorder_array[index1];

		(*sub_root)->parent = *parent;
		for (index2 = start_index; preorder_array[index1] != inorder_array[index2]; index2++)
			;

		preorder_array[index1] = -1;
		if (end_index - start_index <= 1)//此时不能更下一层递归了
			return;
		else
		{
			create_subtree(sub_root, &((*sub_root)->lchild), start_index, index2);//前：包括；后：不包括
			create_subtree(sub_root, &((*sub_root)->rchild), index2 + 1, end_index);
		}


		return;
	}
	else
	{
		return;
	}
}



void create_root(tree *root)
{
    int index1, index2;
    if ((*root) == NULL)
    {
		(*root) = (tree)malloc(sizeof(node));
		(*root)->data = 0;
		(*root)->lchild = (*root)->rchild = (*root)->parent = NULL;
    }
	index1 = 0;
	(*root)->data = preorder_array[index1];
	
	for (index2 = 0; index2 < in_len; index2++)
	{
		if (preorder_array[index1] == inorder_array[index2])
			break;
	}
	preorder_array[index1] = -1;//使用过的根节点数值标记一下，当做哨兵
	create_subtree(root, &((*root)->lchild), 0, index2);//前：包括；后：不包括
	create_subtree(root, &((*root)->rchild), index2 + 1, pre_len);
    return;
}

elemtype visit_array[MAXLENGTH] = { 0 };
int visit_counter = 0;

void Visit(elemtype e)
{
	/*
	if (visit_counter < length - 1)
		printf("%d,", e);
	else
		printf("%d", e);
	visit_counter++;
	*/
	visit_array[visit_counter] = e;
	visit_array[visit_counter + 1] = '\0';
	visit_counter++;
	return;
}

void PostorderTraverse(tree T, void(*Visit)(elemtype e))
{
	if (T)
	{
		PostorderTraverse(T->lchild, Visit);
		PostorderTraverse(T->rchild, Visit);
		Visit(T->data);
	}
	return;
}

void visit_print()
{
	int index;
	for (index = 0; index < visit_counter - 1; index++)
		printf("%d,", visit_array[index]);
	printf("%d", visit_array[index]);
	return;
}

int main()
{
    tree root = NULL;
    create_array();
	create_root(&root);
	PostorderTraverse(root, Visit);
	visit_print();
    return 0;
}