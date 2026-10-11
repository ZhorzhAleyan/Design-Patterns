package com.example.patterns;

import java.util.ArrayList;
import java.util.List;

public class CompositeDemo {
    interface FileSystemNode {
        String name();
        long sizeInBytes();
        void print(String indent);
    }

    static class FileNode implements FileSystemNode {
        private final String name;
        private final long size;
        FileNode(String name, long size) {
            if (size < 0) throw new IllegalArgumentException("Negative size");
            this.name = name;
            this.size = size;
        }
        public String name() { return name; }
        public long sizeInBytes() { return size; }
        public void print(String indent) {
            System.out.println(indent + name + " (" + size + " bytes)");
        }
    }

    static class FolderNode implements FileSystemNode {
        private final String name;
        private final List<FileSystemNode> children = new ArrayList<>();
        FolderNode(String name) { this.name = name; }
        public String name() { return name; }
        void add(FileSystemNode node) {
            if (node == this || (node instanceof FolderNode folder && folder.contains(this))) {
                throw new IllegalArgumentException("Folder cycle is not allowed");
            }
            children.add(node);
        }
        private boolean contains(FileSystemNode target) {
            if (this == target) return true;
            for (FileSystemNode child : children) {
                if (child == target || (child instanceof FolderNode folder && folder.contains(target))) {
                    return true;
                }
            }
            return false;
        }
        public long sizeInBytes() {
            long sum = 0;
            for (FileSystemNode child : children) sum = Math.addExact(sum, child.sizeInBytes());
            return sum;
        }
        public void print(String indent) {
            System.out.println(indent + name + "/ (" + sizeInBytes() + " bytes)");
            for (FileSystemNode child : children) child.print(indent + "  ");
        }
    }

    public static void main(String[] args) {
        FolderNode root = new FolderNode("Documents");
        root.add(new FileNode("notes.txt", 10));
        FolderNode photos = new FolderNode("Photos");
        photos.add(new FileNode("summer.jpg", 100));
        photos.add(new FileNode("winter.jpg", 200));
        root.add(photos);
        root.print("");
        if (root.sizeInBytes() != 310) throw new AssertionError("Wrong folder size");
    }
}
