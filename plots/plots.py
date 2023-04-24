
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
hash_symb   = df['hash_symb'].head(i_size)
hash_strlen = df['hash_strlen'].head(i_size)
hash_ascii  = df['hash_ascii'].head(i_size)
hash_rol    = df['hash_rol'].head(i_size)
hash_ror    = df['hash_ror'].head(i_size)
hash_mrot   = df['hash_mrot'].head(i_size)

# Figure Size
fig = plt.figure(figsize = (10, 4))
 
# Horizontal Bar Plot
#plt.bar(index, hash_cnst,   color ='maroon')
plt.bar(index, hash_symb,   color ='pink')
plt.bar(index, hash_strlen, color ='red')
plt.bar(index, hash_ror,    color ='black')


#plt.bar(index, hash_rol,    color ='green')
#plt.bar(index, hash_mrot,   color ='maroon')

#plt.bar(index, hash_ascii,  color ='blue')

# Add labels
plt.xlabel("Index")
plt.ylabel("Words amount")
plt.title("Hash function distribution")
plt.savefig('plots_pic/plot.png')