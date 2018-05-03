# vnc docker

Start annotatorvnc docker container.

```bash
docker-compose up -d annotatorvnc
```

Find out ip address.

```bash
docker exec -i -t docker_annotatorvnc ip a
```

Open browser and point to '{IP}:6080/vnc.html?autoconnect=true&host={IP}&port=5900&password=annotator'.
