import sys,csv, math
import matplotlib as matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.pyplot import cm
from cycler import cycler
from matplotlib.ticker import ScalarFormatter

# Arguments are [outputfile] [number of cores] [0 - balanced 1 - unbalanceds]
# filenames must be unique you swine
def std_dev(str_lst) :
        avg = average(str_lst)
        total = 0;
        count = len(str_lst);
        for item in str_lst:
                total += (float(item) - avg) * (float(item) - avg)
        return math.sqrt(total/count)

def average(str_lst) :
        total = float(0);
        count = len(str_lst);
        for item in str_lst:
                total += float(item)
        return (total/count)

def get_dataset(filename):
        data = dict();
        iters = dict();
        cores = dict();
        with open(filename, 'rb') as csvfile:
                reader = csv.reader(csvfile, delimiter=',')
                depth_set = dict();
                core_set = dict();
                for row in reader:
                        algorithm = row[0].strip()
                        color = row[1].strip()
                        core = int(row[2])
                        core_set[core] = 0;
                        depth = int(row[3])
                        depth_set[depth] = 0;
                        duration = int(row[4])
                        if not algorithm in data:
                                data[algorithm] = dict();
                        if not core in data[algorithm]:
                                data[algorithm][core] = dict();
                        if not depth in data[algorithm][core] :
                                data[algorithm][core][depth] = list();
                        data[algorithm][core][depth].append(duration);
        return (data, depth_set, core_set)


outputfile = sys.argv[1]
data_set, depth_set, core_set = get_dataset(outputfile)
runtime_results = dict();
for depth_val in sorted(depth_set.keys()):
        print "Results for " + str(depth_val) + " depth" 
        ax = plt.gca()
        formatter = ScalarFormatter()
        formatter.set_scientific(False)
        ax.yaxis.set_major_formatter(formatter)
        color=iter(cm.rainbow(np.linspace(0,1,10)))
        marker=iter(["\\" , "/" , "\\" , "/" ,"\\" , "/" ,"\\" , "/" ,"\\" , "/" ,"\\" , "/" ,"\\" , "/" ,"\\" , "/" ,"\\" , "/" ,"\\" , "/" ])
        m=next(marker)
        c=next(color)
        core_vals = sorted(core_set.keys())
        num_cores=len(core_set.keys())
        num_algorithms = len(data_set.keys())
        bar_width= 1.0/float(num_algorithms + 1)
        indices=np.arange(num_cores)
        runtime_cnt=0
        for runtime in data_set.keys():
                if runtime == "SerialPVS":
                        continue
                if runtime == "AlphaBeta":
                        continue
                if runtime == "SharedHashtable":
                        continue
                runtime_results[runtime] = [0,1,2]
                runtime_results[runtime][0] = np.empty(len(core_vals))
                runtime_results[runtime][1] = np.empty(len(core_vals))
                runtime_results[runtime][2] = np.empty(len(core_vals))
                for i in range (len(core_vals)):
                        runtime_results[runtime][0][i] = core_vals[i]
                        runtime_results[runtime][1][i] = float(average(data_set[runtime][int(core_vals[i])][depth_val]))
                        runtime_results[runtime][2][i] = float(std_dev(data_set[runtime][int(core_vals[i])][depth_val]))
                plt.bar(indices + runtime_cnt*bar_width, runtime_results[runtime][1], bar_width, yerr=runtime_results[runtime][2], label=runtime, hatch=m)
                c=next(color)
                m=next(marker)
                runtime_cnt = runtime_cnt+1


        plt.xlabel('Number of Cores')
        plt.ylabel('Runtime (Microseconds)')
        plt.xticks(indices+((num_algorithms/2)*(bar_width/2)), core_vals)
        plt.title("Running Times For Depth " + str(depth_val))

        plt.legend(loc='best')

        plt.savefig(("depth" + str(depth_val) + ".png"))
        plt.clf()


print "Speedup vs depth"
ax = plt.gca()
formatter = ScalarFormatter()
formatter.set_scientific(False)
ax.yaxis.set_major_formatter(formatter)
color=iter(cm.rainbow(np.linspace(0,1,10)))
marker=iter(["\\" , "/" , "\\" , "/" ,"\\" , "/" ,"\\" , "/" ,"\\" , "/" ,"\\" , "/" ,"\\" , "/" ,"\\" , "/" ,"\\" , "/" ,"\\" , "/" ])
m=next(marker)
c=next(color)
depth_vals = sorted(depth_set.keys())
num_depths=len(depth_set.keys())
num_algorithms = len(data_set.keys())
bar_width= 1.0/float(num_algorithms + 1)
indices=np.arange(num_depths)
runtime_cnt=0
for runtime in data_set.keys():
        if runtime == "SerialPVS":
                continue
        if runtime == "AlphaBeta":
                continue
        runtime_results[runtime] = [0,1,2]
        runtime_results[runtime][0] = np.empty(num_depths)
        runtime_results[runtime][1] = np.empty(num_depths)
        runtime_results[runtime][2] = np.empty(num_depths)
        for i in range (num_depths):
                runtime_results[runtime][0][i] = depth_vals[i]
                runtime_results[runtime][1][i] = float(average(data_set[runtime][1][depth_vals[i]]))/float(average(data_set[runtime][32][depth_vals[i]]))
        plt.bar(indices + runtime_cnt*bar_width, runtime_results[runtime][1], bar_width, label=runtime, hatch=m)
        c=next(color)
        m=next(marker)
        runtime_cnt = runtime_cnt+1


plt.xlabel('Depth')
plt.ylabel('Speedup')
plt.xticks(indices+((num_algorithms/2)*(bar_width/2)), depth_vals)
plt.title("Speedup By Depth")

plt.legend(loc='best')

plt.savefig(("depth_speedup.png"))
plt.clf()

