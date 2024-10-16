# Introdução a Robotica

A porta computacional do robo é `COM10` e o ip para efetuar a comonicação do robo é `192.168.3.101`.

## Odometria e equações de movimento de um robô

Considere o robot móvel disponível para esta unidade curricular. Este desloca-se num plano horizontal por intermédio de duas rodas motrizes diferenciais e com uma terceira roda livre para apoio.

### Constante que relaciona os impulsos com a distância percorrida

#### CINEMÁTICA – TRAÇÃO DIFERENCIAL

$$
\frac{d}{dt}
\begin{bmatrix}
x(t) \\
y(t) \\
\theta(t)
\end{bmatrix}
=
\begin{bmatrix}
v(t) \cos(\theta(t)) \\
v(t) \sin(\theta(t)) \\
w(t)
\end{bmatrix}
$$

$$
\begin{aligned}
v(t) &= \frac{v_1(t) + v_2(t)}{2} \quad & w(t) &= \frac{v_1(t) + v_2(t)}{b} \\
d(i) &= \frac{d_1(i) + d_2(i)}{2} \quad & \Delta\theta(i) &= \frac{d_1(i) - d_2(i)}{b}
\end{aligned}
$$

| Senc 1 | Senc 2 |
| ------ | ------ |
|        |        |

### Distancia entre Rodas.
