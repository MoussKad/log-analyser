Analyseur de Logs en Temps Réel 🛠️📜

Description

Ce programme en C++ permet de surveiller un fichier de log en temps réel sous Linux. Il affiche chaque nouvelle ligne ajoutée et offre une fonctionnalité de filtrage par mot-clé.

Fonctionnalités :

  ✅ Surveillance en temps réel d’un fichier log 
  
  ✅ Filtrage des logs avec un mot-clé donné 
  
  ✅ Utilisation de inotify pour une surveillance efficace 
  

Installation & Compilation

Prérequis

  Ce programme nécessite un système Linux avec g++ installé.

Compilation :

  sh

    g++ -o log_monitor log_monitor.cpp

Exécution :

  🔹 Affichage de tous les logs en temps réel
  
  sh
  
    ./log_monitor /var/log/nginx/access.log
  
  🔹 Filtrage des logs contenant un mot-clé spécifique
  
  sh
  
    ./log_monitor /var/log/nginx/access.log "ERROR"
  
  Ce mode n’affichera que les lignes contenant "ERROR".


Améliorations Futures :


  🔹 Support de plusieurs fichiers logs simultanément 
  
  🔹 Exportation des logs filtrés vers un fichier de sortie 
  
  🔹 Interface Web pour afficher les logs en direct 
