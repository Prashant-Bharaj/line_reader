/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prasingh <prasingh@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 23:41:38 by prasingh          #+#    #+#             */
/*   Updated: 2025/11/29 00:53:14 by prasingh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

char	*get_next_line(int fd);
char	*ft_strchr(const char *s, int c);
char	*ft_strjoin(char *s1, char const *s2);
size_t	ft_strlen(const char *s);

/* per-fd linked list node used internally by get_next_line */
typedef struct s_fdnode
{
	int             fd;
	char            *save;
	struct s_fdnode *next;
}   t_fdnode;

/* helpers to manage per-fd nodes (implemented in utils) */
t_fdnode *get_fd_node(t_fdnode **head, int fd);
void      remove_fd_node(t_fdnode **head, int fd);

#endif
