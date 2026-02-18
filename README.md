# Zex

<p align="center">
  <img src="./logo/zex_final.png" alt="zexZ" width="150" height="150" />
</p>

## Présentation

**Zex** est un éditeur de texte orienté clavier, conçu pour les utilisateurs qui veulent travailler vite, proprement et sans interruption.  
Il privilégie la fluidité des gestes, la continuité de l’attention et l’adaptation complète à chaque flux de travail.

Minimaliste dans son esthétique mais ambitieux dans ses capacités, Zex donne la priorité à l’ergonomie des mains et à l’expressivité des commandes plutôt qu’à l’accumulation de fonctionnalités.

---

## Philosophie

Zex repose sur une approche pragmatique : fournir des **primitives puissantes, claires et orthogonales**.

L’éditeur favorise :
- des actions courtes et déterministes,
- des commandes prévisibles et composables,
- une interaction directe et structurée avec le texte.

L’objectif est de réduire la friction mentale et physique, afin de préserver la concentration sur le contenu.

---

## Interaction avec le texte

L’expérience centrale de Zex est pensée autour de la manipulation du texte :

- Navigation rapide par unités logiques (mots, phrases, blocs)
- Sélections sensibles à la structure et à la sémantique
- Transformations précises et enchaînables
- Recherche et réécriture performantes pour les refactorings et modifications massives

Zex permet de travailler sur de grands volumes de texte sans perdre le fil du raisonnement.

---

## Extensibilité

Extensible par conception, Zex expose des points d’accroche clairs pour enrichir son comportement :

- Macros légères
- Scripts intégrés
- Modules optionnels

L’environnement d’extension est volontairement accessible : écrire une extension reste simple, tout en offrant suffisamment de puissance pour automatiser des tâches récurrentes ou créer des aides contextuelles propres à un langage ou à un projet.

---

## Personnalisation

Zex s’adapte à l’utilisateur, et non l’inverse :

- Mappage de touches entièrement configurable
- Profils de comportement par projet
- Thèmes d’affichage sobres et lisibles

L’éditeur conserve une empreinte mémoire légère et reste réactif même sur de très grands fichiers, afin que la latence n’interrompe jamais la concentration.

---

## Aides visuelles

Bien que l’interface soit épurée par défaut, Zex propose des aides visuelles temporaires, activables au besoin :

- Aperçu des modifications
- Visualisation de l’historique d’annulations
- Miniatures et aides de navigation

Ces outils restent discrets et ne s’imposent jamais au flux de travail.

---

## Intégration système

Zex s’intègre naturellement dans une chaîne d’outils existante :

- Terminal intégré
- Gestion de sessions
- Sauvegarde automatique intelligente
- Interopérabilité avec des outils externes

L’éditeur évite de devenir un silo et favorise une utilisation fluide dans des environnements variés.

---

## En résumé

Zex est un éditeur de texte conçu pour la **productivité**, la **clarté** et la **maîtrise du texte**.
Il accompagne aussi bien le rédacteur ponctuel que le développeur exigeant, en offrant un outil rapide, expressif et durable.

---

## Installation

### Dépendances

```bash
# Debian / Ubuntu
sudo apt install clang libncurses-dev

# Arch Linux
sudo pacman -S clang ncurses

# Fedora
sudo dnf install clang ncurses-devel
```

### Compiler et installer

```bash
git clone https://github.com/<you>/zex
cd zex
make
sudo make install
```

`zex` est maintenant disponible partout sur le système.

### Désinstaller

```bash
sudo make uninstall
```

---

## Utilisation

```bash
# Ouvrir un fichier
zex fichier.c

# Ouvrir un dossier (arborescence à gauche)
zex mon_projet/
```

### Raccourcis

| Touche | Action |
|---|---|
| `Ctrl+X` puis `Ctrl+S` | Sauvegarder |
| `Ctrl+Q` | Quitter |
| `Ctrl+W` | Basculer le focus arbre / éditeur |
| Flèches | Déplacer le curseur |
| `j` / `k` | Naviguer dans l'arbre (focus TREE) |
| `Entrée` | Ouvrir un fichier / plier-déplier un dossier |
