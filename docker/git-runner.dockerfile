FROM debian:bookworm-slim

RUN apt-get update
RUN apt-get install -y curl

WORKDIR /opt/actions-runner
RUN curl -o actions-runner-linux-x64-2.326.0.tar.gz -L https://github.com/actions/runner/releases/download/v2.326.0/actions-runner-linux-x64-2.326.0.tar.gz
RUN tar xzf ./actions-runner-linux-x64-2.326.0.tar.gz
RUN ./bin/installdependencies.sh
RUN chmod -R a+rw /opt/actions-runner

RUN useradd -m -s /bin/bash developer
USER developer

ENTRYPOINT [ "bash", "-c", "./config.sh --url ${RUNNER_REPO} --token ${RUNNER_TOKEN} && ./run.sh" ]