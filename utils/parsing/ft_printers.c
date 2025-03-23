#include "../../includes/Minishell.h"

// Helper function to compute height of the tree.
// A NULL tree has height 0.
static int tree_height(t_parse *root)
{
    if (!root)
        return 0;
    int left_height = tree_height(root->left);
    int right_height = tree_height(root->right);
    return (left_height > right_height ? left_height : right_height) + 1;
}

// Helper function to print a single node's content.
// If the node is NULL or its 'node' field or content is missing,
// it prints "(empty)".
static void print_node(t_parse *node)
{
    if (!node)
    {
        printf("(empty)");
        return;
    }
    if (node->node && node->node->content)
        printf("%s", node->node->content);
    else
        printf("(empty)");
}

// Main function to print the tree level by level.
void ft_print_tree_visual(t_parse *root)
{
    if (!root)
    {
        printf("(empty)\n");
        return;
    }
    
    int height = tree_height(root);
    // current_level will hold the nodes for the current level.
    // Maximum nodes at a given level is 2^(level-1), so we start with 1.
    t_parse **current_level = malloc(sizeof(t_parse *));
    if (!current_level)
        return;
    current_level[0] = root;
    
    // Process each level.
    for (int level = 1; level <= height; level++)
    {
        int num_nodes = 1 << (level - 1);  // 2^(level-1) nodes at this level
        // For the next level, allocate an array with capacity 2^(level).
        t_parse **next_level = NULL;
        if (level < height)
        {
            next_level = malloc(sizeof(t_parse *) * (1 << level));
            if (!next_level)
            {
                free(current_level);
                return;
            }
        }
        int next_index = 0;
        for (int i = 0; i < num_nodes; i++)
        {
            // Print the node content (or (empty) if node is NULL)
            print_node(current_level[i]);
            if (i < num_nodes - 1)
                printf(" ");
            
            // For non-last levels, add children (or NULL placeholders) to next_level.
            if (level < height)
            {
                if (current_level[i])
                {
                    next_level[next_index++] = current_level[i]->left;
                    next_level[next_index++] = current_level[i]->right;
                }
                else
                {
                    // If the current node pointer is NULL, we add two NULLs.
                    next_level[next_index++] = NULL;
                    next_level[next_index++] = NULL;
                }
            }
        }
        printf("\n");
        free(current_level);
        current_level = next_level;
    }
}
