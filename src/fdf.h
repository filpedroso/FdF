/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fdf.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fpedroso <fpedroso@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 18:44:33 by fpedroso          #+#    #+#             */
/*   Updated: 2025/02/20 14:49:44 by fpedroso         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FDF_H
# define FDF_H

//#include "/Users/filpedroso/42/common-core/fdf/resources/minilibx_mms_20200219/mlx.h"
#include "/home/fpedroso/42/common-core/fdf/resources/minilibx-linux/mlx.h"
#include "/home/fpedroso/42/common-core/fdf/resources/minilibx-linux/mlx_int.h"

#define HEIGHT 1300
#define WIDTH 800

typedef struct	s_canvas
{
	void	*connection;
	void	*window;
	void	*image;
	int		**map;
}	t_canvas;



#endif