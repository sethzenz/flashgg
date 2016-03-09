files = ["ggh_full.txt","tth_full.txt","vbf_full.txt","wh_full.txt","zh_full.txt"]

for fn in files:
    f = open(fn,'r')
    name = fn.split("_")[0]
    pdf = []
    alphaS = []
    scale = []
    for line in f.readlines():
        line = line.strip()
        if line.count("scale"):
            scale.append(float(line.strip().split()[-1]))
        if line.count("alpha"):
            alphaS.append(float(line.strip().split()[-1]))
        if line.count("pdf"):
            pdf.append(float(line.strip().split()[-1]))
    print "%s_pdf = %s" % (name,pdf)
    print "%s_alphaS = %s" % (name,alphaS)
    print "%s_scale = %s" % (name,scale)

