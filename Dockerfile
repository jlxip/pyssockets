FROM jlxip/ssockets:0.1.3-docker2 as build

RUN apk add --no-cache git py3-pip gcc python3-dev musl-dev
RUN pip install --no-cache-dir build

RUN git clone https://github.com/jlxip/pyssockets ~/pyssockets
RUN python3 -m build ~/pyssockets --sdist
RUN pip install --no-cache-dir ~/pyssockets/dist/*.tar.gz

# Cleanup
RUN rm -rf ~/pyssockets
RUN pip uninstall -y build tomli pep517 pyparsing retrying
RUN apk del git gcc python3-dev musl-dev

# Flattening
FROM scratch
COPY --from=build / /