s = [1, 0]
t = [0, 1]
i = 1
r = [7111111, 123456]
q = [None]
while(True):
    i+=1
    r.append(r[i-2]%r[i-1])
    q.append((r[i-2] - r[i])/r[i-1])
    s.append(s[i-2] - q[i-1]*s[i-1])
    t.append(t[i-2] - q[i-1]*t[i-1]) 
    if(r[-1] is 0):
        if r[-2] is 1:
            print("multiplicative inverse of %s mod %s is %s"%(r[1], r[0], t[-2]))
        else:
            print("no multiplicative inverse exists for %s mod %s"%(r[1], r[0]))
        break