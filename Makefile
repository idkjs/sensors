# https://www.gnu.org/software/make/manual/html_node/Phony-Targets.html#Phony-Targets
# https://www.gnu.org/software/make/manual/html_node/Force-Targets.html

server.exe: FORCE
	@dune build $(DUNEARGS) server/bin/server.exe

install: FORCE
	@dune install

uninstall: FORCE
	@dune uninstall

clean:
	@dune $(DUNEARGS) clean

cleanall: clean


.PHONY: schema
schema: ## loads schema
	./db/load_schema.sh
.PHONY: db
db: ## Inspect db
	./db/psql.sh

.PHONY: lock
lock: ## Generate a lock file
	opam lock -y .

.PHONY: deps
deps:
	# Install the dependencies
	opam install . --deps-only --with-doc --with-test -y

.PHONY: init
init:
	# Create a local opam switch and install deps
	opam switch create . ocaml-base-compiler.4.11.1 --deps-only --locked
	opam install -y ocaml-lsp-server reason
	make install

.PHONY: fmt
fmt: ## Format the codebase with ocamlformat
	opam exec -- dune build --root . --auto-promote @fmt

.PHONY: watch
watch: ## Watch for the filesystem and rebuild on every change
	opam exec -- dune build --root . --watch


# https://www.gnu.org/software/make/manual/html_node/Phony-Targets.html#Phony-Targets
# https://www.gnu.org/software/make/manual/html_node/Force-Targets.html
FORCE: ;
