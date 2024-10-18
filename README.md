# Introdução a Robotica

A porta computacional do robo é `COM10` e o ip para efetuar a comonicação do robo é `192.168.3.101`.

## Odometria e equações de movimento de um robô

Considere o robot móvel disponível para esta unidade curricular. Este desloca-se num plano horizontal por intermédio de duas rodas motrizes diferenciais e com uma terceira roda livre para apoio.

#### CINEMÁTICA – TRAÇÃO DIFERENCIAL

```math
\frac{d}{dt}
\begin{pmatrix}
x(t) \\
y(t) \\
\theta(t)
\end{pmatrix}
=
\begin{pmatrix}
v(t) \cos(\theta(t)) \\
v(t) \sin(\theta(t)) \\
w(t)
\end{pmatrix}
```

```math
\begin{aligned}
v(t) &= \frac{v_1(t) + v_2(t)}{2} \quad & w(t) &= \frac{v_1(t) + v_2(t)}{b} \\
d(i) &= \frac{d_1(i) + d_2(i)}{2} \quad & \Delta\theta(i) &= \frac{d_1(i) - d_2(i)}{b}
\end{aligned}
```

### Constante que relaciona os impulsos com a distância percorrida

```math
k = \frac{\text{Distance Traveled (m)}}{\text{Impulses (Senc)}}
```

| PWM v1 | PWM v2 | Senc1 | Senc2 | Δx1(m) | k1 = Δx1/Senc1 (m/impulse) | k2 = Δx2/Senc2 (m/impulse) |    k~     |
| :----: | :----: | :---: | :---: | :----: | :------------------------: | :------------------------: | :-------: |
|   30   |   30   | 1663  | 1916  | 0.096  |         5.7727e-5          |         5.0104e-5          | 5.3916e-5 |
|   10   |   10   |  805  |  971  | 0.047  |         5.8385e-5          |         4.8403e-5          | 5.3394e-5 |
|   20   |   20   | 1924  | 2235  | 0.105  |         5.4574e-5          |         4.6980e-5          | 5.0777e-5 |

```math
k = 5.2696 \times 10^{-5} \, \text{(m/Senc)}
```

### Distancia entre as rodas

```math
 b= \frac{d_1(i) - d_2(i)}{\Delta\theta(i)}
```

| PWM v1 | PWM v2 | Senc1 | Senc2 | d1 (m)  | d2 (m)  | Angl(rad) | b (m)  |
| :----: | :----: | :---: | :---: | :-----: | :-----: | :-------: | :----: |
|   30   |  -30   | 6899  | -7849 | 0.36354 | 0.41361 |  $2\pi$   | 0.1196 |
|   10   |  -10   | 6432  | -7831 | 0.33894 | 0.41266 |  $2\pi$   | 0.1241 |
|   20   |  -20   | 6839  | -7964 | 0.36038 | 0.41967 |  $2\pi$   | 0.1236 |

```math
b = 0.1225 \, \text{(m)}
```
