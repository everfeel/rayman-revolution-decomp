.include "build/gp_externs.inc"
.set noreorder

.macro glabel label
    .globl \label
    .type \label, @function
    \label:
    .ent \label
.endm

.macro endlabel label
    .size \label, . - \label
    .end \label
.endm

.macro ehlabel label
    .globl \label
    \label:
.endm

.macro alabel label
    .globl \label
    .type \label, @function
    \label:
.endm

.macro jlabel label
    .globl \label
    \label:
.endm

.macro dlabel label
    .globl \label
    .type \label, @object
    \label:
.endm

.macro enddlabel label
    .size \label, . - \label
.endm

.macro nonmatching label, size=1
    # Retail function size is retained in the source for matching checks.
.endm
