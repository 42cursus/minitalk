/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlst1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abelov <abelov@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/29 20:07:49 by abelov            #+#    #+#             */
/*   Updated: 2023/12/29 20:07:51 by abelov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Allocates (with malloc(3)) and returns a new dlist.
 * The member variable 'data' is initialized with the value
 * of the parameter 'data'.
 * The variable 'next' is initialized to NULL.
 * The variable 'next' is initialized to NULL.
 */
t_ilst	*ft_dlst_new(void)
{
	t_ilst	*lst;

	lst = (t_ilst *)malloc(sizeof(t_ilst) * 1);
	if (!lst)
		return (NULL);
	lst->head = NULL;
	lst->tail = NULL;
	lst->size = 0;
	return (lst);
}

t_dnode	*ft_dnode_new(int value)
{
	t_dnode	*dnode;

	dnode = (t_dnode *) malloc(sizeof (t_dnode) * 1);
	if (!dnode)
		return (NULL);
	dnode->value = value;
	dnode->next = NULL;
	dnode->prev = NULL;
	return (dnode);
}

void	ft_dlist_foreach(t_ilst *list, t_func func, void *user_data)
{
	t_dnode	*elem;
	t_dnode	*next;

	elem = list->head;
	while (elem)
	{
		next = elem->next;
		(*func)(&elem->value, user_data);
		elem = next;
	}
}

void	ft_dlist_free(t_ilst *list)
{
	t_dnode	*elem;
	t_dnode	*next;

	if (!list)
		return ;
	elem = list->head;
	while (elem)
	{
		next = elem->next;
		free(elem);
		elem = next;
	}
}

void	ft_dlist_free_full(t_ilst **list)
{
	if (!list)
		return ;
	ft_dlist_free(*list);
	free(*list);
	*list = NULL;
}
