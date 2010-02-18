% This example provides an implementation of Conway's famous game of life.
% It uses the SMG module from the modules directory.  Make sure you 
% build it first.

import ("smg");

% This is a simple random number generator
private variable _urand_seed = _time() / (getpid () + 1);
private define _urand (unused)
{
   _urand_seed =  _urand_seed * 69069UL + 1013904243UL;
   return _urand_seed / 4294967296.0;
}

private define urand (m, n)
{
   variable a = array_map (Double_Type, &_urand, [1:m*n]);
   reshape (a, [m, n]);
   return a;
}

% The algorithm for the game begins here.
private define make_left (n)
{
   variable a;

   a = Int_Type [n]; 
   a[0] = n-1;
   a[[1:]] = [0:n-2];
   
   return a;
}

private define make_right(n)
{
   variable a;

   a = Int_Type [n]; 
   a[[0:n-2]] = [1:n-1];
   a[-1] = 0;
   
   return a;
}


private define life_init (nr, nc)
{
   variable a;
   variable up, down, left, right;
   variable i;
   variable num_neighbors;

   a = typecast (5.0 * urand (nr, nc), Char_Type);
   a[where (a != 1)] = 0;

   up = make_left (nr);
   down = make_right (nr);
   left = make_left (nc);
   right = make_right (nc);

   return (a,up,down,left,right);
}

private define life_new_generation (a, up, down, left, right)
{
   variable b;
   variable middle = [:];
   variable i;

   % Make sure array contains only 0 and 1
   a[where(a)] = 1;

   b = (a[up,left] + a[up, middle] + a[up, right]
	+ a[middle,left] + a[middle, right]
	+ a[down, left] + a[down, middle] + a[down, right]);
   b = typecast (b, Char_Type);

   i = where ((b < 2) or (b > 3) or ((b == 2) and (a == 0)));
   b[i] = 0;
   return b;
}

define life_print (a, old_a)
{
   variable dims, i, j;
   (dims,,) = array_info (a);
   
   a = @a;
   a[where (a)] = 1;
   a[where (a and (old_a == 0))] = 2;

   smg_set_color (0);
   smg_cls ();

   _for (0, dims[0]-1, 1)
     {
	i = ();
	foreach (where (a[i,*]))
	  {
	     j = ();
	     smg_gotorc(i,j);
	     smg_set_color (a[i,j]);
	     smg_write_string ("O");
	  }
     }
   smg_set_color (0);
   smg_refresh ();
   sleep (1);
}

define life (nr, nc)
{
   variable a, left, right, up, down, new_a;
    
   (new_a, up, down, left, right) = life_init (nr, nc);
   
   a = new_a;
   do
     {
	life_print (new_a, a);
	a = new_a;
	new_a = life_new_generation (a, up, down, left, right);
     }
   while (length (where (new_a)));
}

smg_init_smg ();

life (Smg_Screen_Rows, Smg_Screen_Cols);

   
