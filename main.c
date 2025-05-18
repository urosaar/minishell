// #include "minishell.h"

// int main(int ac, char **av, char **envp)
// {
//     t_env *env =NULL;
//     char *input;
//     char **splited;
//     int i;
//     // int id;

//     // id = fork();
//     // if (!id)
//     // {
        
//     // }
//     copy_env(envp, &env);
//     while (1)
//     {

//         input = readline("minishell$ ");
//         if (!input)
//             break;
//         splited = ft_split(input, ' ');
//         if (!splited)
//         {
//             printf ("hii\n");
//             free(input);
//             return(0);
//         }
//         if (*splited && !ft_strcmp(*splited, "echo"))
//             ft_echo(splited);
//         else if (*splited && !ft_strcmp(*splited, "pwd"))
//             ft_pwd();
//         else if (*splited && !ft_strcmp(*splited, "cd"))
//             ft_cd(&env, splited);
//         else if (*splited && !ft_strcmp(*splited, "exit"))
//             ft_exit(env, splited);
//         else if (*splited && !ft_strcmp(*splited, "env"))
//             ft_env(env);
//         else if (*splited && !ft_strcmp(*splited, "unset"))
//             ft_unset(&env, splited);
//         else if (*splited && !ft_strcmp(*splited, "export"))
//             ft_export(&env, splited);
//         else
//         {
//             if (*splited)
//                 printf("minishell: %s: command not found\n", *splited);
//             continue;
//         }
//         add_history(input);
//         free(input);
//     } 
//     return (0);
// }
