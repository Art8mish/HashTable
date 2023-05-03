
# Import the libraries
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

i_size = 1009

data = pd.read_csv('data/data.csv', sep=';')
data.head()
df = pd.DataFrame(data)

#print(df.describe())

index       = df['index'].head(i_size)
hash_cnst   = df['hash_cnst'].head(i_size)
hash_symb   = df['hash_symb'].head(200)
hash_strlen = df['hash_strlen'].head(200)
hash_ascii  = df['hash_ascii'].head(i_size)
hash_rol    = df['hash_rol'].head(i_size)
hash_ror    = df['hash_ror'].head(200)
hash_mrot   = df['hash_mrot'].head(i_size)

# Figure Size
fig = plt.figure(figsize = (10, 4))
 
# plt.bar(index, hash_cnst,   color ='purple', label='cnst')
# plt.bar(index, hash_symb,   color ='pink',  label='symb')
# plt.bar(index, hash_strlen, color ='black', label='strlen')
# plt.bar(index, hash_ror,    color ='red',   label='ror')

plt.bar(index, hash_rol,    color='green',  label='rol')
plt.bar(index, hash_mrot,   color='maroon', label='addmul')
plt.bar(index, hash_ascii,  color='blue',  label='ascii')

plt.legend()

# Add labels
plt.xlabel("index")
plt.ylabel("words amount")
plt.title("Hash function distribution")
plt.savefig('plots_pic/plot.png')