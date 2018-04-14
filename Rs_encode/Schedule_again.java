	for ( int i = 0; i < Q; i++ )	
 		{
 			avail = class_avail( plan_start[i], plan_end[i], class_start, class_end );
 			dupli_days = n_days;
 			int count = 0;
 			
 			for ( int j =0; j < N; j++ )
 				{
 					if ( avail[j] == 0 )
 						dupli_n_days = -1;
 					else 
 						count++;
 				}
 			// sort all the available classes according to their no days.
 			
 				int min_daysfirst_classes[] = new int[count]; 
 				int n_days_available_class[] = new int[count];
 				int array_iter =0;
 			for ( int j = 0; j < N; j++ )

 			{
 				if ( avail[ j ] == 1 )
 				{
 					min_daysfirst_classes[ array_iter ++] = j; 
 					n_days_available_class[array_iter++] = dupli_days;
 				}
 			}
 				
			int temp = 0, pos = 0;
			int swap = 0;
			
			for ( int j = 0; j < count; j++ )
			{	temp = n_days_available_class[j];
				pos = j;
				for ( int k = j+1; k < count; k++)
				{	if ( n_days_available_class[k] < temp)
							{
								temp = n_days_available_class[k];
								pos = k;
							}
				
				}			
				
				swap = pos;
				pos  = min_daysfirst_classes[j];
				min_daysfirst_classes[j] = swap;

				swap = n_days_available_class[j];
				n_days_available_class[ j ] = temp;
				n_days_available_class[ pos ] = swap;

			}
