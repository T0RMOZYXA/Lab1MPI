import numpy as np
import glob
import pandas as pd

def read_matrix(filename):
    with open(filename) as f:
        lines = f.readlines()
    n = int(lines[0].strip())
    mat = []
    for line in lines[1:]:
        mat.extend([float(x) for x in line.split()])
    return np.array(mat).reshape(n, n)

def verify_all():
    result_files = glob.glob("C_*.txt")
    result_files.sort(key=lambda x: int(x.split('_')[1].split('.')[0]))
    
    data = []
    for cfile in result_files:
        N = int(cfile.split('_')[1].split('.')[0])
        print(f"\n=== N = {N} ===")
        A = read_matrix(f"A_{N}.txt")
        B = read_matrix(f"B_{N}.txt")
        C_prog = read_matrix(cfile)
        
        C_ref = np.dot(A.astype(np.float64), B.astype(np.float64))
        
        abs_diff = np.abs(C_ref - C_prog)
        max_abs = np.max(abs_diff)
        mean_abs = np.mean(abs_diff)
        rel_diff = abs_diff / (np.abs(C_ref) + 1e-15)
        max_rel = np.max(rel_diff)
        
        print(f"max_abs = {max_abs:.2e}, mean_abs = {mean_abs:.2e}, max_rel = {max_rel:.2e}")
        
        data.append({
            'N': N,
            'max_abs': max_abs,
            'mean_abs': mean_abs,
            'max_rel': max_rel
        })
    
    df = pd.DataFrame(data)
    print("\nСводная таблица:")
    print(df.to_string(index=False))
    df.to_csv("table_res.csv", index=False)

if __name__ == "__main__":
    verify_all()
