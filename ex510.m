load("-ascii","da509");
d = da509;
my = size(d)(1);
mx = size(d)(2);

dc = cumsum(d')';
th5 = ones(my,1);
th0 = [th5*mx/2, th5*10];

function [dJ] = partCost(dc, y, x, mx)
  dJ = 0;
  if (x >= 1)
    if (x <= mx)
      dJ = interp1(dc(y,:), x, 'linear', 'extrap');
    else
      dJ = dc(y, mx);
    endif
  endif 
end


function [J] = cost(theta, gama, my, mx, dc)
  J = 0;
  
  center = theta(1:my);
  width = theta(my+1:end);
  J = J +  gama * sum(width);
  for y = 1:my
    x0 = center(y)-width(y)/2;
    x1 = center(y)+width(y)/2;
    J = J - (partCost(dc, y, x1, mx) - partCost(dc, y, x0, mx));
  endfor

end

cost(th0,20, my, mx, dc)