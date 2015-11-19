tot = 0
changed = 0
changed10 = 0
changed15 = 0
inc10 = 0
inc15 = 0

for line in open("current_baseline.txt",'r'):
    if line.count("pT(H)"):
        tot += 1
        nums = []
        for item in line.strip().split():
            try:
                x = float(item)
                nums.append(x)
            except Exception:
                continue
        nums = nums[2:]
        if nums[0] != nums[1] or nums[0] != nums[2] or nums[1] != nums[2]:
            changed += 1
            print nums
            if (nums[1] != nums[0]):
                changed10 += 1
            if (nums[2] != nums[0]):
                changed15 += 1
            if nums[1] > nums[0]:
                inc10 +=1
            if nums[2] > nums[0]:
                inc15 +=1
        
print "changed10/tot = %i/%i" % (changed10,tot)
print "inc10/tot = %i/%i" % (inc10,tot)
print "changed15/tot = %i/%i" % (changed15,tot)
print "inc15/tot = %i/%i" % (inc15,tot)


        
