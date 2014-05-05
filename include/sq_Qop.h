/*
 * vim:expandtab:shiftwidth=4:tabstop=4:
 *
 * Copyright   (2014)      Contributors
 * Contributor : chitr   chitr.prayatan@gmail.com
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 * 
 * ---------------------------------------
 */


Q_RETURN_t
sq_init(CB_Q *p_queue,uint64_t q_size,cb_process p_get_memory,cb_process p_free_memory);
Q_RETURN_t 
sq_enqueue(CB_Q *p_queue,Q_NODE *p_node);
Q_NODE *
sq_dequeue(CB_Q *p_queue);
int  
sq_isempty(CB_Q *p_queue);
int  
sq_isfull(CB_Q *p_queue);
int 
sq_size(CB_Q *p_queue);
Q_RETURN_t 
sq_destroy(CB_Q *p_queue);
