/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: prasingh <prasingh@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 22:04:41 by prasingh          #+#    #+#             */
/*   Updated: 2025/12/06 16:37:17 by prasingh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_BONUS_H
# define GET_NEXT_LINE_BONUS_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

char				*get_next_line(int fd);
char				*ft_strchr(const char *s, int c);
char				*ft_strjoin(char *s1, char const *s2);
size_t				ft_strlen(const char *s);

typedef struct s_fdnode
{
	int				fd;
	char			*save;
	struct s_fdnode	*next;
}					t_fdnode;

t_fdnode			*get_fd_node(t_fdnode **head, int fd);
void				remove_fd_node(t_fdnode **head, int fd);

#endif
