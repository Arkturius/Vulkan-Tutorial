/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rgramati <rgramati@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 01:33:24 by rgramati          #+#    #+#             */
/*   Updated: 2024/05/22 00:10:59 by rgramati         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLORS_H
# undef COLORS_H
# ifndef __COLORS_H
#  define __COLORS_H

#  define BOLD        		"\e[1m"
#  define DIM         		"\e[2m"
#  define ITALIC			"\e[3m"
#  define UNDERLINED  		"\e[4m"
#  define BLINK       		"\e[5m"
#  define REVERSE     		"\e[7m"
#  define HIDDEN      		"\e[8m"

#  define RESET_ALL         "\e[0m"
#  define RESET_BOLD        "\e[21m"
#  define RESET_DIM         "\e[22m"
#  define RESET_UNDERLINED  "\e[24m"
#  define RESET_BLINK       "\e[25m"
#  define RESET_REVERSE     "\e[27m"
#  define RESET_HIDDEN      "\e[28m"

#  define FG_DEFAULT  		"\e[39m"
#  define FG_BLACK    		"\e[30m"
#  define FG_RED      		"\e[31m"
#  define FG_GREEN    		"\e[32m"
#  define FG_YELLOW   		"\e[33m"
#  define FG_BLUE     		"\e[34m"
#  define FG_MAGENTA  		"\e[35m"
#  define FG_CYAN     		"\e[36m"
#  define FG_LGRAY    		"\e[37m"
#  define FG_DGRAY    		"\e[90m"
#  define FG_LRED     		"\e[91m"
#  define FG_LGREEN   		"\e[92m"
#  define FG_LYELLOW  		"\e[93m"
#  define FG_LBLUE    		"\e[94m"
#  define FG_LMAGENTA 		"\e[95m"
#  define FG_LCYAN    		"\e[96m"
#  define FG_WHITE    		"\e[97m"

#  define BG_DEFAULT  		"\e[49m"
#  define BG_BLACK    		"\e[40m"
#  define BG_RED      		"\e[41m"
#  define BG_GREEN    		"\e[42m"
#  define BG_YELLOW   		"\e[43m"
#  define BG_BLUE     		"\e[44m"
#  define BG_MAGENTA  		"\e[45m"
#  define BG_CYAN     		"\e[46m"
#  define BG_LGRAY    		"\e[47m"
#  define BG_DGRAY    		"\e[100m"
#  define BG_LRED     		"\e[101m"
#  define BG_LGREEN   		"\e[102m"
#  define BG_LYELLOW  		"\e[103m"
#  define BG_LBLUE    		"\e[104m"
#  define BG_LMAGENTA 		"\e[105m"
#  define BG_LCYAN    		"\e[106m"
#  define BG_WHITE    		"\e[107m"

# endif // __COLORS_H
#endif // COLORS_H