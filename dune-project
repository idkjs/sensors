(lang dune 2.0)

(generate_opam_files true)

(package
 (name sensors)
 (synopsis
  "Minimalist framework to build extensible HTTP servers and clients")
 (description
  "Rock is a Unix indpendent API to build extensible HTTP servers and clients. It provides building blocks such as middlewares and handlers (a.k.a controllers).")
 (depends
  (ocaml
   (>= 4.11.0))
  (dream
   (>= 1.0.0~alpha2))
  (caqti
   (>= 1.6.0))
  (caqti-lwt
   (>= 1.6.0))
  (caqti-driver-postgresql
   (>= 1.6.0))
  ;; JSON
  (ppx_yojson_conv
   (>= v0.14.0))
  ptime
  re
  (reason
   (and
    (>= 3.6.0)
    (< 4.0.0)))
  ;; Dev dependencies
  (ocaml-lsp-server :dev)))
