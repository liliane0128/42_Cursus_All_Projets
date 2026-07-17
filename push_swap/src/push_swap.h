/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lilwang <lilwang@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/19 08:41:26 by lilwang           #+#    #+#             */
/*   Updated: 2025/07/19 11:00:02 by lilwang          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include "libft.h"
# include <stdio.h>

typedef struct s_list
{
	long			content;
	int				index;
	struct s_list	*next;
}				t_list;

/*operations*/
void	pa(t_list **stack_a, t_list **stack_b);
void	pb(t_list **stack_a, t_list **stack_b);
void	rra(t_list **stack_a);
void	rrb(t_list **stack_b);
void	rrr(t_list **stack_a, t_list **stack_b);
void	ra(t_list **stack_a);
void	rb(t_list **stack_b);
void	rr(t_list **stack_a, t_list **stack_b);
void	sa(t_list **stack_a);
void	sb(t_list **stack_b);
void	ss(t_list **stack_a, t_list **stack_b);

/*sort*/
void	sort_chunks(t_list **a, t_list **b);
void	push_chunks_to_b(t_list **a, t_list **b, int chunk_size);
void	push_back_to_a(t_list **a, t_list **b);
int		find_index_pos(t_list *stack, int obj_index);
int		is_sorted(t_list *stack);
void	sort_3(t_list **stack_a);
void	sort_5(t_list **stack_a, t_list **stack_b);

/*utils*/
long	ft_atol(char *s);
t_list	*lst_new_node(long int value);
int		lst_add_back(t_list **lst, t_list *new);
void	free_split(char **split);
void	free_stack(t_list **stack);
void	assign_index(t_list *lst);
int		parse_args(int ac, char **av, t_list **a);
int		ft_lstsize(t_list *lst);
t_list	*ft_lstlast(t_list *lst);

#endif
